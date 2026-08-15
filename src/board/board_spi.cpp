#include <Arduino.h> 
#include <SPI.h>              /* 包含SPI库 */
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "board/board_pins.h"  /* 包含板级引脚定义 */
#include "board/board_spi.h"   /* 包含板级SPI总线模块 */
#include <Wire.h>

#include "board/board_pins_pcb1_mcp23017.h"
#include "hal/mcp23017_u3.h"
#include "hal/board_hw_control.h"

SPIClass SPI_SD;              /* SD专用SPI类实例 */
static SemaphoreHandle_t s_ui_spi_mtx = nullptr;

/* 初始化板级SPI总线 - 初始化默认SPI和SD专用SPI */
void board_spi_init(void)
{
    static bool inited = false;  /* 静态标志，确保初始化只执行一次 */
    if (inited) return;          /* 如果已经初始化则直接返回 */
    inited = true;

    Serial.println("[启动] 初始化SPI总线...");

    // ---------- I2C（触摸 FT6336G / 无 MCP23017） ----------
    Wire.begin(board::PIN_I2C_SDA, board::PIN_I2C_SCL);
    Wire.setClock(400000);

    // ES3C28P 无 MCP23017，桩实现返回 false，跳过扩展芯片初始化。
    const bool mcp_ok = mcp23017_u3_begin();
    mcp23017_u3_debug_dump();

    // 背光（GPIO45）由 board_hw_control 独立控制，与 MCP 无关，始终初始化。
    board_hw_control_begin();

    if (!mcp_ok) {
        Serial.println("[总线] ES3C28P 无 MCP23017，跳过 I2C 扩展芯片初始化");
    } else {
        board_hw_debug_dump();
    }

    if (!s_ui_spi_mtx) {
        s_ui_spi_mtx = xSemaphoreCreateRecursiveMutex();
    }

    // ---------- Chip Select safe state ----------
    if (PIN_TFT_CS >= 0) {
        pinMode(PIN_TFT_CS, OUTPUT);
        digitalWrite(PIN_TFT_CS, HIGH);
    }

    // ES3C28P 无 RC522 / NFC，CS 引脚为 -1，跳过。
    if (PIN_RC522_CS >= 0) {
        pinMode(PIN_RC522_CS, OUTPUT);
        digitalWrite(PIN_RC522_CS, HIGH);
    }

    if (PIN_SD_CS >= 0) {
        pinMode(PIN_SD_CS, OUTPUT);
        digitalWrite(PIN_SD_CS, HIGH);
    }

    // ---------- UI SPI: TFT ----------
    // SS 参数务必用 -1（别传 TFT_CS）
    ::SPI.end();
    ::SPI.begin(PIN_SPI_UI_SCK, PIN_SPI_UI_MISO, PIN_SPI_UI_MOSI, -1);

    // ---------- SD SPI ----------
    SPI_SD.end();
    SPI_SD.begin(PIN_SPI_SD_SCK, PIN_SPI_SD_MISO, PIN_SPI_SD_MOSI, -1);

    Serial.printf("[总线] 初始化UI和SDSPI总线参数: SCK=%d MOSI=%d MISO=%d\n",
                  PIN_SPI_UI_SCK, PIN_SPI_UI_MOSI, PIN_SPI_UI_MISO);
    Serial.printf("[总线] 屏幕：CS=%d DC=%d 背光=GPIO%d\n",
                  PIN_TFT_CS,
                  PIN_TFT_DC,
                  PIN_TFT_BL);
    Serial.printf("[总线] SD卡 时钟引脚=%d 主出从入引脚=%d 主入从出引脚=%d 芯片选择引脚=%d\n",
                  PIN_SPI_SD_SCK,
                  PIN_SPI_SD_MOSI,
                  PIN_SPI_SD_MISO,
                  PIN_SD_CS);
}

void board_spi_ui_lock(void)
{
    if (s_ui_spi_mtx) {
        xSemaphoreTakeRecursive(s_ui_spi_mtx, portMAX_DELAY);
    }
}

void board_spi_ui_unlock(void)
{
    if (s_ui_spi_mtx) {
        xSemaphoreGiveRecursive(s_ui_spi_mtx);
    }
}
