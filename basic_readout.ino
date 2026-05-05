/*
 * MPU6050 Basic Readout
 * ---------------------
 * Reads raw accelerometer, gyroscope, and temperature data from an
 * MPU6050 (GY-521 breakout) over I2C and prints it to the Serial Monitor.
 *
 * Target board: Arduino Uno (or any 5V AVR Arduino)
 * Wiring:
 *   MPU6050 VCC  -> Arduino 5V
 *   MPU6050 GND  -> Arduino GND
 *   MPU6050 SCL  -> Arduino A5
 *   MPU6050 SDA  -> Arduino A4
 *   MPU6050 INT  -> (not used)
 *
 * Open Serial Monitor at 9600 baud after upload.
 *
 * (c) 2026 Circuitrocks. Released under the MIT License.
 * https://github.com/circuitrocks
 */

#include <Wire.h>

// MPU6050 I2C address (AD0 pin low = 0x68, high = 0x69)
const uint8_t MPU_ADDR = 0x68;

// Register addresses (from the MPU-6000/6050 Register Map datasheet)
const uint8_t REG_PWR_MGMT_1   = 0x6B;
const uint8_t REG_ACCEL_XOUT_H = 0x3B;

// Sensitivity scale factors for the default ranges
// Accelerometer: +/- 2g  -> 16384 LSB/g
// Gyroscope:    +/- 250 deg/s -> 131 LSB/(deg/s)
const float ACCEL_SCALE = 16384.0f;
const float GYRO_SCALE  = 131.0f;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Wake the MPU6050: it powers up in sleep mode, so we clear the SLEEP bit
  // by writing 0x00 to PWR_MGMT_1.
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(REG_PWR_MGMT_1);
  Wire.write(0x00);
  uint8_t status = Wire.endTransmission(true);

  if (status != 0) {
    Serial.println(F("MPU6050 not found. Check wiring and power."));
    while (1) {
      delay(1000);
    }
  }

  Serial.println(F("MPU6050 ready."));
  Serial.println(F("aX\taY\taZ (g)\tgX\tgY\tgZ (deg/s)\tTemp (C)"));
}

void loop() {
  int16_t ax, ay, az, rawTemp, gx, gy, gz;

  // Request 14 bytes starting at ACCEL_XOUT_H. The MPU6050 auto-increments
  // its internal pointer, so we get accel (6) + temp (2) + gyro (6) in one read.
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(REG_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, (uint8_t)14, (uint8_t)true);

  ax = (Wire.read() << 8) | Wire.read();
  ay = (Wire.read() << 8) | Wire.read();
  az = (Wire.read() << 8) | Wire.read();
  rawTemp = (Wire.read() << 8) | Wire.read();
  gx = (Wire.read() << 8) | Wire.read();
  gy = (Wire.read() << 8) | Wire.read();
  gz = (Wire.read() << 8) | Wire.read();

  // Convert raw values to physical units
  float accelX = ax / ACCEL_SCALE;
  float accelY = ay / ACCEL_SCALE;
  float accelZ = az / ACCEL_SCALE;
  float gyroX  = gx / GYRO_SCALE;
  float gyroY  = gy / GYRO_SCALE;
  float gyroZ  = gz / GYRO_SCALE;

  // Temperature formula from the datasheet: T = (raw / 340) + 36.53
  float tempC = (rawTemp / 340.0f) + 36.53f;

  Serial.print(accelX, 2); Serial.print('\t');
  Serial.print(accelY, 2); Serial.print('\t');
  Serial.print(accelZ, 2); Serial.print('\t');
  Serial.print(gyroX, 2);  Serial.print('\t');
  Serial.print(gyroY, 2);  Serial.print('\t');
  Serial.print(gyroZ, 2);  Serial.print('\t');
  Serial.println(tempC, 2);

  delay(500);
}
