# Smart-City
# Smart City Automation System using Arduino Uno

## Overview
This project implements an automated smart city model using Arduino Uno, integrating multiple IoT systems for urban management. The system includes real-time parking management, automatic street lighting, environmental monitoring like fire detection, and smart waste management solutions. All sensor data is streamed to Firebase and visualized on a web dashboard for real-time monitoring.

## Demonstration Video
<div align="center"> <video width="80%" controls> <source src="Video.mp4" type="video/mp4"> Your browser does not support the video tag. </video> <p><em>System demonstration showing all features in operation</em></p> </div>

## Features

### 1. Smart Parking System
- **LED display** showing available parking spots
- **Automated barriers** controlling vehicle access
- **Ultrasonic sensors** detecting vehicle 
- **Rain sensor** detects rain and shade comes on parking area
- **Real-time updates** to Firebase and web dashboard

### 2. Adaptive Street Lighting
- **LDR sensors** detecting ambient light levels
- **Weather-adaptive** intensity control
- **Motion sensors** for energy conservation
- **Automatic on/off** based on daylight conditions

### 3. Environmental Monitoring
- **Fire detection** using flame sensor
- **Rain detection** using rain sensor
- **Emergency alerts** for hazardous conditions

### 4. Smart Dustbins
- **Ultrasonic sensors** measuring waste levels
- **LED indicators** showing fill status
- **Compaction mechanism** (simulated)

## Hardware Components
- Arduino Uno (x4 - one for each subsystem)
- Ultrasonic sensors (HC-SR04)
- IR sensors for parking
- Servo motors (for barriers)
- Rain Sensor
- LDR sensors
- Flame sensor
- LEDs
- Breadboards and jumper wires
- Power supplies

## Software Components
- Arduino IDE
- Firebase Realtime Database
- Web dashboard (HTML/CSS/JavaScript)
- Firebase SDK for data streaming
- Libraries:
  - Servo.h
  - Wire.h
  - lcd.h

## Setup Instructions

### Hardware Setup
1. Connect all sensors to their respective Arduino boards following the circuit diagrams
2. Ensure proper power supply to all components

### Software Setup
1. Install required Arduino libraries
2. Configure Firebase credentials in each Arduino sketch
3. Set up Firebase Realtime Database with appropriate data structure
4. Upload individual sketches to each Arduino board
5. Deploy web dashboard to host or local server

### Firebase Configuration
1. Create a Firebase project
2. Set up Realtime Database with these nodes:
   - `/parking`
   - `/environment`

## Web Dashboard
The interactive dashboard displays:
- Real-time parking availability
- Alert notifications

## Usage
1. Power on all Arduino boards
2. Open the web dashboard in browser
3. Monitor real-time parking status
4. Receive alerts for critical events
   

## Troubleshooting
- **Sensor readings inaccurate**: Check connections and calibrate sensors
- **Firebase connection issues**: Verify WiFi credentials and Firebase rules
- **Dashboard not updating**: Check internet connection and Firebase configuration
- **Actuators not responding**: Verify power supply and signal connections

## Future Enhancements
- Implement solar power for streetlights
- Add license plate recognition for parking
  

