import serial
import firebase_admin
from firebase_admin import credentials, db
from datetime import datetime
import json

# Initialize Firebase
cred = credentials.Certificate("serviceAccountKey.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': ''
})

# Get database reference
ref = db.reference('parking_system')

# Serial port configuration
ser = serial.Serial('COM3', 9600, timeout=1)
print("Listening for Arduino data on", ser.name)

def send_to_firebase(data):
    try:
        # Add timestamp to data
        data["timestamp"] = datetime.now().isoformat()
        
        # Send to Firebase
        if "environment" in data:
            ref.child('environment').set(data["environment"])
            print("Environment data sent to Firebase:", data["environment"])
        else:
            ref.set(data)
            print("Data sent to Firebase:", data)
    except Exception as e:
        print("Firebase error:", e)

def main():
    while True:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                
                if line.startswith('{') and line.endswith('}'):
                    try:
                        data = json.loads(line)
                        send_to_firebase(data)
                    except json.JSONDecodeError as e:
                        print("JSON decode error:", e, "| Raw data:", line)
        except KeyboardInterrupt:
            print("Exiting...")
            ser.close()
            break
        except Exception as e:
            print("Error:", e)
            ser.close()
            break

if __name__ == "__main__":
    main()