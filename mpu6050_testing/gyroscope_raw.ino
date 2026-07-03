#include <Wire.h>

#define MPU_ADDR 0x68

int16_t GyroX, GyroY, GyroZ;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // ----- MPU6050 Initialization -----
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);     // Wake up sensor
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Configure gyroscope range to ±250 °/s
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1B);     // GYRO_CONFIG register
  Wire.write(0x00);     // ±250dps
  Wire.endTransmission(true);

  Serial.println("MPU6050 Initialized for Raw Gyroscope Readings");
}

void loop() {
  // Request gyroscope registers starting from 0x43
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  GyroX = (Wire.read() << 8) | Wire.read();
  GyroY = (Wire.read() << 8) | Wire.read();
  GyroZ = (Wire.read() << 8) | Wire.read();

  Serial.print("GyroX: "); Serial.print(GyroX);
  Serial.print(" | GyroY: "); Serial.print(GyroY);
  Serial.print(" | GyroZ: "); Serial.println(GyroZ);

  delay(200);
}
