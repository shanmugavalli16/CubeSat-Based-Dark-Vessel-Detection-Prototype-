#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "00001";

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B

char text[32];

float yaw = 0;
unsigned long prevTime = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  // Wake MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  // NRF Setup
  if (radio.begin())
    Serial.println("NRF DETECTED");
  else
    Serial.println("NRF NOT DETECTED");

  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);

  radio.openWritingPipe(address);
  radio.stopListening();

  prevTime = millis();

  Serial.println("TX READY");
}

void loop()
{
  int16_t raw_acc_x, raw_acc_y, raw_acc_z;
  int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU6050_ADDR, 14, true);

  raw_acc_x = (Wire.read() << 8) | Wire.read();
  raw_acc_y = (Wire.read() << 8) | Wire.read();
  raw_acc_z = (Wire.read() << 8) | Wire.read();

  Wire.read();
  Wire.read();

  raw_gyro_x = (Wire.read() << 8) | Wire.read();
  raw_gyro_y = (Wire.read() << 8) | Wire.read();
  raw_gyro_z = (Wire.read() << 8) | Wire.read();

  float ax = raw_acc_x / 16384.0;
  float ay = raw_acc_y / 16384.0;
  float az = raw_acc_z / 16384.0;

  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
  float roll  = atan2(ay, az) * 180.0 / PI;

  float gz = raw_gyro_z / 131.0;

  unsigned long now = millis();
  float dt = (now - prevTime) / 1000.0;
  prevTime = now;

  yaw += gz * dt;

  sprintf(text,
          "P:%d R:%d Y:%d",
          (int)pitch,
          (int)roll,
          (int)yaw);

  radio.write(text, sizeof(text));

  Serial.print("Sent: ");
  Serial.println(text);

  delay(200);
}
