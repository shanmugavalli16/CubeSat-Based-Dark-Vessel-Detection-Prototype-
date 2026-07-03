// SATELLITE ARDUINO
// Receives heartbeat from Vessel
// Reads MPU6050 Pitch/Roll/Yaw
// Sends Orientation + Vessel Status to Ground Station

#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);

// Vessel -> Satellite
const byte vesselAddr[6] = "00001";

// Satellite -> Ground
const byte groundAddr[6] = "00002";

#define MPU6050_ADDR 0x68
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B

char rxText[32];
char txText[50];

float yaw = 0;
unsigned long prevTime = 0;
unsigned long lastHeartbeat = 0;

String vesselStatus = "LOST";

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  // MPU6050 Init
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission();

  // NRF Init
  if (radio.begin())
    Serial.println("NRF DETECTED");
  else
    Serial.println("NRF NOT DETECTED");

  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);

  prevTime = millis();

  Serial.println("SATELLITE READY");
}

void loop()
{
  // ==========================
  // RECEIVE FROM VESSEL (5 sec)
  // ==========================

  radio.openReadingPipe(0, vesselAddr);
  radio.startListening();

  unsigned long rxStart = millis();

  while (millis() - rxStart < 5000)
  {
    if (radio.available())
    {
      memset(rxText, 0, sizeof(rxText));
      radio.read(rxText, sizeof(rxText));

      Serial.print("Received: ");
      Serial.println(rxText);

      if (strcmp(rxText, "VESSEL_OK") == 0)
      {
        lastHeartbeat = millis();
        vesselStatus = "OK";
      }
    }
  }

  if (millis() - lastHeartbeat > 5000)
  {
    vesselStatus = "LOST";
  }

  // ==========================
  // READ MPU6050
  // ==========================

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

  // ==========================
  // SEND TO GROUND (1 sec)
  // ==========================

  sprintf(txText,
          "P:%d R:%d Y:%d V:%s",
          (int)pitch,
          (int)roll,
          (int)yaw,
          vesselStatus.c_str());

  radio.stopListening();

  radio.openWritingPipe(groundAddr);

  for (int i = 0; i < 5; i++)
  {
    radio.write(txText, sizeof(txText));

    Serial.print("Sent: ");
    Serial.println(txText);

    delay(200);
  }
}
