#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Servo setup
Servo gateServo;
Servo shelterServo;  // New servo for the shelter

// Sensor pins (with pull-up)
const int enterPin = A0;
const int exitPin = A1;
const int park1Pin = A2;
const int park2Pin = A3;
const int rainSensorPin = A4;  // Analog pin for rain sensor

// Variables
bool entryDetected, exitDetected;
bool p1Occupied, p2Occupied;
int availableSlots = 2;
unsigned long lastGateOperation = 0;
const unsigned long gateCooldown = 1500; // 1.5 seconds between operations
bool isRaining = false;
unsigned long lastRainCheck = 0;
const unsigned long rainCheckInterval = 5000; // Check every 5 seconds

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Parking System");
  delay(2000);
  lcd.clear();
  
  // Set pin modes with pull-up
  pinMode(enterPin, INPUT_PULLUP);
  pinMode(exitPin, INPUT_PULLUP);
  pinMode(park1Pin, INPUT_PULLUP);
  pinMode(park2Pin, INPUT_PULLUP);
  pinMode(rainSensorPin, INPUT);  // Rain sensor doesn't need pull-up
  
  // Attach servos
  if(gateServo.attach(8)) { // Check if servo attached successfully
    gateServo.write(0); // Initial position (closed)
    Serial.println("Gate Servo initialized");
  } else {
    Serial.println("Gate Servo failed to attach!");
  }
  
  if(shelterServo.attach(9)) { // Shelter servo on pin 9
    shelterServo.write(0); // Initial position (shelter open)
    Serial.println("Shelter Servo initialized");
  } else {
    Serial.println("Shelter Servo failed to attach!");
  }
}

void loop() {
  // Read sensors (LOW = detected)
  entryDetected = !digitalRead(enterPin);
  exitDetected = !digitalRead(exitPin);
  p1Occupied = !digitalRead(park1Pin);
  p2Occupied = !digitalRead(park2Pin);

  // Update available slots
  availableSlots = 2 - (p1Occupied + p2Occupied);

  // Display on LCD
  updateDisplay();

  // Entry logic with cooldown check
  if (entryDetected && availableSlots > 0 && (millis() - lastGateOperation) > gateCooldown) {
    operateGate();
    lastGateOperation = millis();
  }

  // Exit logic with cooldown check
  if (exitDetected && (millis() - lastGateOperation) > gateCooldown) {
    operateGate();
    lastGateOperation = millis();
  }

  // Rain detection logic
  if (millis() - lastRainCheck > rainCheckInterval) {
    checkRain();
    lastRainCheck = millis();
  }

  // Serial debugging
  Serial.print("P1:"); Serial.print(p1Occupied);
  Serial.print(" P2:"); Serial.print(p2Occupied);
  Serial.print(" Slots:"); Serial.print(availableSlots);
  Serial.print(" Entry:"); Serial.print(entryDetected);
  Serial.print(" Exit:"); Serial.print(exitDetected);
  Serial.print(" Rain:"); Serial.println(isRaining);

  delay(100); // Reduced delay for better responsiveness
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  lcd.print("Available: ");
  lcd.print(availableSlots);
  
  lcd.setCursor(0, 1);
  lcd.print("P1:");
  lcd.print(p1Occupied ? "Full " : "Free");
  lcd.print(" P2:");
  lcd.print(p2Occupied ? "Full" : "Free");
  
  // Show rain status on LCD if raining
  if (isRaining) {
    lcd.setCursor(13, 0);
    lcd.print("RAIN");
  } else {
    lcd.setCursor(13, 0);
    lcd.print("    "); // Clear rain status
  }
}

void operateGate() {
  if(gateServo.attached()) { // Check if servo is properly attached
    gateServo.write(0);  // Open gate
    delay(3000);
    gateServo.write(90);   // Close gate
    delay(500);
  } else {
    Serial.println("Warning: Gate Servo not attached!");
  }
}

void checkRain() {
  int rainValue = analogRead(rainSensorPin);
  // Lower value means more rain (typical for rain sensors)
  bool rainingNow = (rainValue < 500); // Adjust threshold as needed
  
  if (rainingNow && !isRaining) {
    // Rain just started - activate shelter
    if (shelterServo.attached()) {
      shelterServo.write(90); // Move to covered position
      Serial.println("Rain detected - shelter activated");
    } else {
      Serial.println("Warning: Shelter Servo not attached!");
    }
    isRaining = true;
  } 
  else if (!rainingNow && isRaining) {
    // Rain stopped - deactivate shelter
    if (shelterServo.attached()) {
      shelterServo.write(0); // Return to open position
      Serial.println("Rain stopped - shelter deactivated");
    }
    isRaining = false;
  }
}