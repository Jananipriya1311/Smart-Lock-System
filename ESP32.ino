/***************************************************
 * SMART LOCK SYSTEM - ESP32 + BLYNK + ULTRASONIC + SERVO
 ***************************************************/
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Lock System"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// ==== BLYNK & WIFI DETAILS ====
// Replace these with your own credentials
char auth[] = "YOUR_BLYNK_DEVICE_TOKEN";   // Device token
char ssid[] = "YOUR_WIFI_SSID";             // WiFi name
char pass[] = "YOUR_WIFI_PASSWORD";         // WiFi password

// ==== PIN CONFIGURATION ====
#define TRIG_PIN 14
#define ECHO_PIN 27
#define SERVO_PIN 2

Servo lockServo;

long duration_us;
int distance_cm;
unsigned long lastPresence = 0;

const int PRESENCE_THRESHOLD_CM = 50;               // distance to detect person
const unsigned long PRESENCE_COOLDOWN_MS = 7000;    // 7 seconds cooldown

// ==== BLYNK VIRTUAL PINS ====
BLYNK_WRITE(V1) {
  int value = param.asInt();
  Serial.print("Blynk button value: ");
  Serial.println(value);
  
  if (value == 1) unlockDoor();
  else lockDoor();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== SMART LOCK (ESP32) STARTED ===");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  lockServo.attach(SERVO_PIN);
  lockDoor();

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  Blynk.begin(auth, ssid, pass);
  Serial.println("✅ Connected to WiFi & Blynk.");

  // Test notification on startup
  Blynk.logEvent("person_detected", "🔔 System Ready!");
}

void loop() {
  Blynk.run();
  checkUltrasonic();
  delay(500);
}

void checkUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration_us == 0) return;

  distance_cm = duration_us * 0.034 / 2.0;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  if (distance_cm > 0 && distance_cm < PRESENCE_THRESHOLD_CM) {
    unsigned long now = millis();
    if (now - lastPresence > PRESENCE_COOLDOWN_MS) {
      lastPresence = now;
      Serial.println("👤 Person detected near door! Sending event...");

      // Send to Blynk
      Blynk.logEvent("person_detected", String("Person detected at ") + distance_cm + " cm");
      Blynk.virtualWrite(V2, 1);   // signal for camera
      delay(3000);
      Blynk.virtualWrite(V2, 0);   // reset
    }
  }
}

void unlockDoor() {
  Serial.println("🟢 Unlocking door...");
  lockServo.write(90);
  Serial.println("🔓 Door Unlocked");
}

void lockDoor() {
  Serial.println("🔴 Locking door...");
  lockServo.write(0);
  Serial.println("🔒 Door Locked");
}
