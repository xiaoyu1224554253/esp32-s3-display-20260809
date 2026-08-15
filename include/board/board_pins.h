#pragma once

#include "board/board_pins_pcb1_mcp23017.h"

// ============================================================
// 兼容旧代码的宏名：直接使用数值，避免预处理器 #if 表达式
// 展开出 "board::xxx" 导致 ":: not valid in preprocessor" 错误
// ============================================================

// UI SPI: TFT + RC522 共用
#define PIN_SPI_UI_SCK   (12)
#define PIN_SPI_UI_MISO  (-1)
#define PIN_SPI_UI_MOSI  (11)

#define PIN_TFT_CS       (10)
#define PIN_TFT_DC       (46)
#define PIN_TFT_RST      (-1)   // TFT_RST 已迁移到 MCP23017 B3
#define PIN_TFT_BL       (45)

#define PIN_RC522_CS     (-1)
#define PIN_RC522_RST    (-1)   // RC522_RST 已迁移到 MCP23017 B2
#define PIN_RC522_IRQ    (-1)

// SD SPI
#define PIN_SPI_SD_SCK   (38)
#define PIN_SPI_SD_MISO  (39)
#define PIN_SPI_SD_MOSI  (40)
#define PIN_SD_CS        (47)

// Encoder / keys on ESP32
#define PIN_EC06_A       (-1)
#define PIN_EC06_B       (-1)
#define PIN_POWER_PLAY   (0)

// I2S
#define PIN_I2S_BCLK     (5)
#define PIN_I2S_DOUT     (6)
#define PIN_I2S_LRCK     (7)
#define PIN_I2S_MCLK     (4)
#define PIN_I2S_DIN      (8)
#define PIN_AUDIO_EN     (1)

// Other
#define PIN_WS2812       (42)
#define PIN_POWER_CTRL   (-1)
#define PIN_BAT_ADC      (-1)
#define PIN_HALL_OUT     (-1)

// 触摸 I2C（FT6336G）
#define PIN_TOUCH_SDA    (16)
#define PIN_TOUCH_SCL    (15)
#define PIN_I2C_SDA      (16)
#define PIN_I2C_SCL      (15)

// 按键
#define PIN_KEY_BOOT     (0)
