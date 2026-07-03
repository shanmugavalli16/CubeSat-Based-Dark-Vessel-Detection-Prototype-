#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

char data = 'H';

void setup()
{
  Serial.begin(9600);

  if (!radio.begin())
  {
    Serial.println("NRF NOT DETECTED");
    while (1);
  }

  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);

  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("TX READY");
}

void loop()
{
  bool ok = radio.write(&data, sizeof(data));

  if (ok)
    Serial.println("TX SUCCESS");
  else
    Serial.println("TX FAILED");

  delay(1000);
}
