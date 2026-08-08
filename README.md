# ESP32-S3 LCD Display Project

This project is based on the ESP32-S3 with a 2.8 inch LCD display from LCDWiki, featuring touch functionality.

## Hardware
- ESP32-S3 Development Board
- 2.8 inch LCD Display (Touch enabled)
- LCDWiki 2.8inch_ESP32-S3_Display

## Features
- PlatformIO configured for ESP32-S3
- TFT Display support with Adafruit GFX and ILI9341 libraries
- Touch screen support
- Ready for IoT projects

## Project Structure
```
.
├── platformio.ini      # PlatformIO configuration
├── .gitignore          # Git ignore rules
├── README.md           # Project documentation
└── src/
    └── main.cpp        # Main Arduino sketch
```

## Building
Using PlatformIO IDE:
1. Open the project in PlatformIO IDE
2. Press the Build button (Ctrl+Alt+B)
3. Upload to ESP32-S3 board (Ctrl+Alt+U)

Using Command Line:
```bash
pio run
pio run --target upload
```

## Serial Monitor
Monitor the serial output at 115200 baud:
```bash
pio device monitor
```

## Pin Configuration
- TFT_CS: 5
- TFT_DC: 2
- TFT_RST: 4

## License
MIT License
