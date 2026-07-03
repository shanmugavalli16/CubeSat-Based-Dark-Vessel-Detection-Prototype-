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

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Wake MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  radio.begin();
  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);

  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("TX READY");
}

void loop() {

  int16_t ax, ay, az;

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU6050_ADDR, 6, true);

  ax = (Wire.read() << 8) | Wire.read();
  ay = (Wire.read() << 8) | Wire.read();
  az = (Wire.read() << 8) | Wire.read();

  sprintf(text, "AX:%d AY:%d", ax, ay);

  radio.write(text, sizeof(text));

  Serial.print("Sent: ");
  Serial.println(text);

  delay(500);
}
