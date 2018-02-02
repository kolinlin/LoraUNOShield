#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// LoRa Settings 
// #define BAND 434500000.00
#define BAND 915E6
#define spreadingFactor 9
// #define SignalBandwidth 62.5E3
#define SignalBandwidth 31.25E3

#define codingRateDenominator 8

void setup() {
  Serial.begin(115200);
  while (!Serial);
// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // clearDisplay the buffer.
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  Serial.println("LoRa Receiver");
  
  display.println("LoRa Receiver");
  display.display();
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    display.setCursor(0,10);
    display.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  
  Serial.print("LoRa Frequency: ");
  Serial.println(BAND);
  
  Serial.print("LoRa Spreading Factor: ");
  Serial.println(spreadingFactor);
  LoRa.setSpreadingFactor(spreadingFactor);
  
  Serial.print("LoRa Signal Bandwidth: ");
  Serial.println(SignalBandwidth);
  LoRa.setSignalBandwidth(SignalBandwidth);

  LoRa.setCodingRate4(codingRateDenominator);
  display.setCursor(0,10);
  display.println("LoRa Initializing OK!");
  display.display();
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println( "Received packet ");
    display.display();
    // read packet
    while (LoRa.available()) {
      String data = LoRa.readString();
      Serial.print(data);
      display.setCursor(0,10);
      display.println( data);
      display.display();
    }

    // print RSSI of packet
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.print(" with SNR ");
    Serial.println(LoRa.packetSnr());
    display.setCursor(0,20);
    display.println((String)LoRa.packetRssi() + "dB (" + (String)LoRa.packetSnr() +"dB)");
        
    display.display();
  }
}
