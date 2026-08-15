#pragma once

#include <Arduino.h>
#include <stdint.h>

// ============================================================
// LCDWiki ES3C28P 开发板引脚定义（ESP32-S3 + 2.8寸 ILI9341 + FT6336G 触摸）
// ============================================================
namespace board {

// ---- 显示 SPI (TFT, 4-Line SPI) ----
// ILI9341 方屏 240x320，引脚见 LCDWiki ES3C28P 原理图
static constexpr int PIN_SPI_CLK   = 12;   // TFT_CLK
static constexpr int PIN_SPI_MOSI  = 11;   // TFT_DIN
static constexpr int PIN_SPI_MISO  = -1;   // 4-Line SPI 无 MISO
static constexpr int PIN_TFT_CS    = 10;   // TFT_CS
static constexpr int PIN_TFT_DC    = 46;   // TFT_DC
static constexpr int PIN_TFT_RST   = -1;   // 复用 EN 复位
static constexpr int PIN_TFT_BL    = 45;   // 背光控制

// ---- TF 卡 (MicroSD, 板载 SDIO 布线，这里用 SPI 模式复用到 SDIO 引脚) ----
// SDMMC: CLK=IO38 CMD=IO40 D0=IO39 D1=IO41 D2=IO48 D3=IO47
// SPI 模式: SCK=CLK(38) MOSI=CMD(40) MISO=D0(39) CS=D3(47)
static constexpr int PIN_SD_SCK   = 38;
static constexpr int PIN_SD_MOSI  = 40;
static constexpr int PIN_SD_MISO  = 39;
static constexpr int PIN_SD_CS    = 47;

// ---- I2S 音频 (板载音频编解码, IO1=EN, IO4=MCLK, IO5=BCLK, IO6=DOUT, IO7=LRCK, IO8=DIN) ----
static constexpr int PIN_I2S_BCLK  = 5;
static constexpr int PIN_I2S_DOUT  = 6;
static constexpr int PIN_I2S_LRCK  = 7;
static constexpr int PIN_I2S_MCLK  = 4;
static constexpr int PIN_I2S_DIN   = 8;    // 麦克风输入，播放不需要
static constexpr int PIN_AUDIO_EN  = 1;    // 音频编解码使能

// ---- 触摸 I2C (FT6336G) ----
static constexpr int PIN_TOUCH_SDA = 16;
static constexpr int PIN_TOUCH_SCL = 15;

// ---- I2C 总线别名（触摸/外设共用）----
static constexpr int PIN_I2C_SDA = PIN_TOUCH_SDA;
static constexpr int PIN_I2C_SCL = PIN_TOUCH_SCL;

// ---- 按键 ----
static constexpr int PIN_KEY_BOOT  = 0;    // 唯一实体按键

// ---- LED ----
static constexpr int PIN_RGB       = 42;   // 三色灯

// ---- 以下为兼容旧代码宏名（ES3C28P 无对应外设，置 -1 表示未连接/禁用）----
static constexpr int PIN_EXP_INTA   = -1;
static constexpr int PIN_WS2812     = 42;
static constexpr int PIN_POWER_CTRL = -1;
static constexpr int PIN_POWER_PLAY = 0;
static constexpr int PIN_BAT_ADC    = -1;
static constexpr int PIN_HALL_OUT   = -1;
static constexpr int PIN_NFC_IRQ    = -1;
static constexpr int PIN_NFC_CS     = -1;
static constexpr int PIN_EC06_A     = -1;
static constexpr int PIN_EC06_B     = -1;

// UART
static constexpr int PIN_UART0_RX = 44;
static constexpr int PIN_UART0_TX = 43;

}  // namespace board
