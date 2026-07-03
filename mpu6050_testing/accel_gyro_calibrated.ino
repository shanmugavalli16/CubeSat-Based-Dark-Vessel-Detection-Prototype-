#include <Wire.h>

#define MPU6050_ADDR 0x68

// MPU6050 Register Addresses
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Wake up MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);   // Clears sleep bit, enables the sensor
  Wire.endTransmission();

  // Sample Rate Divider
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(SMPLRT_DIV);
  Wire.write(0x07);   // Sample rate = 1kHz / (7+1) = 125Hz
  Wire.endTransmission();

  // Digital Low Pass Filter (DLPF)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(CONFIG);
  Wire.write(0x03);   // DLPF = 44Hz
  Wire.endTransmission();

  // Gyroscope configuration
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(GYRO_CONFIG);
  Wire.write(0x00);   // ±250°/s
  Wire.endTransmission();

  // Accelerometer configuration
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_CONFIG);
  Wire.write(0x00);   // ±2g
  Wire.endTransmission();

  Serial.println("MPU6050 Initialized Successfully");
}

void loop() {
  int16_t raw_acc_x, raw_acc_y, raw_acc_z;
  int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);

  // Reading accelerometer raw values
  raw_acc_x = (Wire.read() << 8) | Wire.read();
  raw_acc_y = (Wire.read() << 8) | Wire.read();
  raw_acc_z = (Wire.read() << 8) | Wire.read();

  // Reading temperature (not used)
  Wire.read(); Wire.read();

  // Reading gyroscope raw values
  raw_gyro_x = (Wire.read() << 8) | Wire.read();
  raw_gyro_y = (Wire.read() << 8) | Wire.read();
  raw_gyro_z = (Wire.read() << 8) | Wire.read();

  // Convert raw values to g
  float ax = raw_acc_x / 16384.0;
  float ay = raw_acc_y / 16384.0;
  float az = raw_acc_z / 16384.0;

  // Convert raw values to degrees per second
  float gx = raw_gyro_x / 131.0;
  float gy = raw_gyro_y / 131.0;
  float gz = raw_gyro_z / 131.0;

  // Print converted values
  Serial.print("Accel (g): ");
  Serial.print(ax); Serial.print(", ");
  Serial.print(ay); Serial.print(", ");
  Serial.println(az);

  Serial.print("Gyro (°/s): ");
  Serial.print(gx); Serial.print(", ");
  Serial.print(gy); Serial.print(", ");
  Serial.println(gz);

  Serial.println("----------------------------");
  delay(200);
}
