#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,10);

const byte address[6] = "00001";

char text[] = "VESSEL_OK";

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);

  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("VESSEL TX READY");
}

void loop() {

  radio.write(text, sizeof(text));

  Serial.println("Sent: VESSEL_OK");

  delay(1000);
}
