#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_TouchScreen.h>

#define TFT_CS 10
#define TFT_DC 46
#define TFT_CLK 12
#define TFT_DIN 11
#define TFT_RST -1
#define TFT_BL 45

#define TS_MINX 300
#define TS_MAXX 3800
#define TS_MINY 300
#define TS_MAXY 3800

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST, TFT_CLK, TFT_DIN);
TouchScreen ts = TouchScreen(TS_MINX, TS_MINY, TS_MAXX, TS_MAXY, 300);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing ESP32-S3 Display...");

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  
  Serial.println("Display initialized successfully!");
  
  testDisplay();
  testTouch();
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

void testTouch() {
  Serial.println("Testing touch screen...");
  
  tft.fillScreen(ILI9341_BLACK);
  
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(10, 10);
  tft.println("Touch Screen Test");
  tft.println("===================");
  tft.println("Touch anywhere to see coords");
  tft.println("===================");
  
  bool touched = false;
  TSPoint p = ts.getPoint();
  
  if (p.z > 10) {
    touched = true;
    
    int x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    int y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    
    tft.fillRect(10, 50, 220, 50, ILI9341_BLUE);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(20, 75);
    tft.print("X: ");
    tft.print(x);
    tft.print(" Y: ");
    tft.println(y);
    
    tft.fillRect(10, 110, 220, 50, ILI9341_GREEN);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(20, 135);
    tft.println("Touched!");
  }
  
  if (!touched) {
    tft.fillRect(10, 50, 220, 50, ILI9341_GRAY);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(20, 75);
    tft.println("Waiting for touch...");
  }
  
  Serial.println("Touch test completed!");
}
