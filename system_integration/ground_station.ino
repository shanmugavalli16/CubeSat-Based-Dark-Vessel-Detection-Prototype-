#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);

const byte address[6] = "00002";

char text[50];

void setup()
{
  Serial.begin(9600);

  if (radio.begin())
    Serial.println("NRF DETECTED");
  else
    Serial.println("NRF NOT DETECTED");

  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);

  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("GROUND STATION READY");
}

void loop()
{
  if (radio.available())
  {
    memset(text, 0, sizeof(text));

    radio.read(text, sizeof(text));

    Serial.print("Received: ");
    Serial.println(text);
  }
}
