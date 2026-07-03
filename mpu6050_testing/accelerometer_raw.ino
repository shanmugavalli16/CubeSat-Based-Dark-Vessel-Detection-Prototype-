#include <Wire.h>

#define MPU_ADDR 0x68  // Default I2C address of MPU6050

int16_t AccX, AccY, AccZ;

void setup() {
  Wire.begin();               // Start I2C communication
  Serial.begin(9600);         // Start Serial Monitor

  // ----- MPU6050 Initialization -----
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);           // PWR_MGMT_1 register
  Wire.write(0x00);           // Wake up MPU6050 (set sleep = 0)
  Wire.endTransmission(true);

  // Set accelerometer range to ±2g (most stable)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);           // ACCEL_CONFIG register
  Wire.write(0x00);           // ±2g range
  Wire.endTransmission(true);

  // Set sample rate (optional)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x19);           // SMPLRT_DIV register
  Wire.write(0x07);           // Sample rate = 1kHz / (7+1) = 125Hz
  Wire.endTransmission(true);

  Serial.println("MPU6050 Initialized for Raw Accelerometer Readings");
}

void loop() {
  // Request accelerometer registers starting from 0x3B
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  // Read raw data (16-bit signed)
  AccX = (Wire.read() << 8) | Wire.read();
  AccY = (Wire.read() << 8) | Wire.read();
  AccZ = (Wire.read() << 8) | Wire.read();

  Serial.print("AccX: "); Serial.print(AccX);
  Serial.print(" | AccY: "); Serial.print(AccY);
  Serial.print(" | AccZ: "); Serial.println(AccZ);

  delay(200);
}
