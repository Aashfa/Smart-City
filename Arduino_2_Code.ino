#include <Servo.h>

// Define pins
const int trigPin = 9;      // Ultrasonic sensor trigger pin
const int echoPin = 10;     // Ultrasonic sensor echo pin
const int servoPin = 11;    // Servo motor control pin
const int flamePin = A1;    // Flame sensor analog pin (changed from A0)
const int buzzerPin = 12;   // Buzzer digital pin
const int ldrPin = A0;      // LDR light sensor (no resistor)
const int ledPin = 13;      // LED for automatic lighting
const int ledPin2 = 7;      // LED for automatic lighting


// Constants
const int detectionRange = 5;       // Detection range in cm
const int openAngle = 90;           // Lid open angle (degrees)
const int closeAngle = 180;         // Lid closed angle (degrees)
const unsigned long openDuration = 2000; // Time lid stays open (ms)
const int flameThreshold = 100;     // Flame detection threshold
const unsigned long alarmDuration = 5000; // Buzzer duration (ms)
const int lightThreshold = 900;     // LDR threshold (higher = more sensitive to dark)

// Variables
Servo lidServo;                     // Servo object
unsigned long lastDetectionTime = 0;
unsigned long fireDetectionTime = 0;
bool lidOpen = false;
bool fireDetected = false;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Configure pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(flamePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ldrPin, INPUT_PULLUP);    // Enable internal pullup for LDR
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // Attach servo and close lid initially
  lidServo.attach(servoPin);
  lidServo.write(closeAngle);
  delay(500); // Give servo time to move
  lidServo.detach(); // Save power when not moving
  
  Serial.println("Smart Bin with Automatic Lighting Initialized");
}

void loop() {
  // 1. Handle automatic lighting based on LDR
  handleAutomaticLighting();
  
  // 2. Handle trash detection and lid movement
  handleLidMovement();
  
  // 3. Check for fire and handle buzzer
  checkForFire();
  sendSmokeAlert()
  
  // Small delay to prevent excessive measurements
  delay(100);
}

void handleAutomaticLighting() {
  int ldrValue = analogRead(ldrPin);
  
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  
  if (ldrValue > lightThreshold) {  // If dark
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);     // Turn LED ON
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);      // Turn LED OFF
  }
}

void handleLidMovement() {
  // Measure distance
  float distance = measureDistance();
  
  // Check if object is within detection range
  if (distance > 0 && distance <= detectionRange) {
    if (!lidOpen) {
      // Open the lid
      lidServo.attach(servoPin);
      lidServo.write(openAngle);
      lidOpen = true;
      Serial.println("Lid opened");
    }
    lastDetectionTime = millis(); // Update last detection time
  }
  
  // Check if it's time to close the lid
  if (lidOpen && (millis() - lastDetectionTime >= openDuration)) {
    lidServo.write(closeAngle);
    delay(500); // Wait for servo to reach position
    lidServo.detach();
    lidOpen = false;
    Serial.println("Lid closed");
  }
}

void checkForFire() {
  int flameValue = analogRead(flamePin);
  Serial.print("Flame sensor value: ");
  Serial.println(flameValue);
  
  // If flame detected (value below threshold)
  if (flameValue < flameThreshold) {
    if (!fireDetected) {
      fireDetected = true;
      fireDetectionTime = millis();
      Serial.println("Fire detected! Sounding alarm.");
      tone(buzzerPin, 1000); // 1kHz tone
    }
  } else {
    // If no flame detected but buzzer is still on
    if (fireDetected && (millis() - fireDetectionTime >= alarmDuration)) {
      noTone(buzzerPin);
      fireDetected = false;
      Serial.println("Fire alarm stopped.");
    }
  }
}

void sendSmokeAlert() {
  Serial.print("{\"smoke\":");
  Serial.print(fireDetected ? "true" : "false");
  Serial.println("}");
}
}