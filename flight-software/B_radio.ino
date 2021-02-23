#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h>

#define LED 2

#define RFM95_CS 15
#define RFM95_RST 16
#define RFM95_INT 5
#define RF95_FREQ 915.0

String telemetry;
int packetnum = 0;

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup_radio() {
  // Declare pins
  pinMode(LED, OUTPUT);
  digitalWrite(LED,HIGH);   
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  
  // Check radio to confirm initialized
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ);
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95 / 96/97/98 modules using the transmitter pin PA_BOOST, then
  // you can set transmission powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

void build_telemetry_datagram() {
  packetnum += 1;
  DynamicJsonDocument datagram(200);
  datagram["id"] = packetnum;
  datagram["dt"] = millis();
  datagram["ax"] = 48.756083;
  datagram["ay"] = 2.302038;
  datagram["az"] = 57.098342;
  telemetry = "";
  serializeJson(datagram, telemetry);
}
