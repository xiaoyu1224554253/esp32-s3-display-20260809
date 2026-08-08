# ESP32-S3 LCD Display Project

This project is based on the ESP32-S3 with a 2.8 inch LCD display from LCDWiki, featuring touch functionality.

## Hardware
- ESP32-S3 Development Board (Xtensa LX7 32-bit dual-core, 240MHz)
- 2.8 inch IPS TFT Display (240x320 resolution, ILI9341V driver)
- Capacitive Touch Screen (FT6336G driver, 240x320 resolution)
- LCDWiki ES3C28P Display Module with Touch

## Features
- PlatformIO configured for ESP32-S3
- TFT Display support with Adafruit GFX and ILI9341 libraries
- Capacitive Touch Screen support with FT6336 driver
- 4-Line SPI interface for display
- I2C interface for touch screen
- Ready for IoT projects

## Pin Configuration

### Display (SPI 4-Line)
- TFT_CS: IO10 (Chip Select)
- TFT_DC: IO46 (Data/Command)
- TFT_CLK: IO12 (Clock)
- TFT_DIN: IO11 (Data Input)
- TFT_RST: -1 (Internal reset)
- TFT_BL: IO45 (Backlight control)

### Touch Screen (I2C)
- I2C_SCL: IO15 (Clock)
- I2C_SDA: IO16 (Data)

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

## Display Test
The project includes two test functions:
1. **testDisplay()** - Tests the LCD display with colorful text and graphics
2. **testTouch()** - Tests the capacitive touch screen and shows touch coordinates

## License
MIT License
