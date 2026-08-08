#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 4

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing ESP32-S3 Display...");

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  
  Serial.println("Display initialized successfully!");
  
  testDisplay();
}

void loop() {
  
}

void testDisplay() {
  Serial.println("Testing display...");
  
  tft.fillScreen(ILI9341_BLACK);
  
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(20, 20);
  tft.println("ESP32-S3 Display");
  
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(20, 60);
  tft.println("LCDWiki 2.8 inch");
  
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(20, 80);
  tft.println("Touch Enabled");
  
  tft.setTextColor(ILI9341_CYAN);
  tft.setCursor(20, 110);
  tft.println("PlatformIO Ready");
  
  tft.fillRect(20, 140, 200, 50, ILI9341_RED);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(40, 165);
  tft.println("Hello World!");
  
  Serial.println("Display test completed!");
}
