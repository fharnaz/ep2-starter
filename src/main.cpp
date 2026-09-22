/*
 * Homework — Tilt switches, knob dims
 *
 * My chosen axis and threshold: inverted Y axis, threshold = +6.0 m/s^2.
 * My readings: UP ≈ +9.55, SIDEWAYS ≈ +3.96, DOWN ≈ +1.71 m/s^2.
 * Testing surprise: The LED switches off when the board is tilted away from UP.
 */

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>

#define POT_PIN 4
#define LED_PIN 40

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(LED_PIN, OUTPUT);

  // MPU-6050: SDA = GPIO8, SCL = GPIO9
  Wire.begin(8, 9);

  if (!mpu.begin()) {
    Serial.println("MPU-6050 not found!");
    while (true) {
      delay(1000);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  Serial.println("Tilt + knob LED ready");
}

void loop() {
  // Read MPU-6050
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);

  // Read potentiometer
  int raw = analogRead(POT_PIN);

  // Convert 0-4095 to 0-255
  int duty = raw / 16;

  if (duty > 255) {
    duty = 255;
  }

  // Invert Y so UP is the positive direction
  float upAxis = -a.acceleration.y;

  // LED works only when the board is pointing UP
  if (upAxis > 6.0) {
    analogWrite(LED_PIN, duty);
  } else {
    analogWrite(LED_PIN, 0);
  }

  // Print readings
  Serial.printf(
    "X %.2f  Y %.2f  upAxis %.2f  raw %d  duty %d\n",
    a.acceleration.x,
    a.acceleration.y,
    upAxis,
    raw,
    duty
  );

  delay(50);
}