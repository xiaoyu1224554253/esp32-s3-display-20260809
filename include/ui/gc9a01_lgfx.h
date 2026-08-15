#pragma once
#include <LovyanGFX.hpp>
#include "board/board_pins.h"

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341 _panel;
  lgfx::Bus_SPI _bus;

public:
  LGFX() {
    { // SPI bus with DMA support
      auto cfg = _bus.config();
      cfg.spi_host   = SPI2_HOST;
      cfg.spi_mode   = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 8000000;              // 先保守一点
      cfg.pin_sclk   = PIN_SPI_UI_SCK;
      cfg.pin_mosi   = PIN_SPI_UI_MOSI;
      cfg.pin_miso   = -1;                   // 4-Line SPI 无 MISO
      cfg.pin_dc     = PIN_TFT_DC;
      cfg.use_lock   = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }
    { // panel: ILI9341 240x320 (LCDWiki ES3C28P 2.8寸方屏)
      auto cfg = _panel.config();
      cfg.pin_cs  = PIN_TFT_CS;
      cfg.pin_rst = PIN_TFT_RST;
      cfg.pin_busy = -1;
      cfg.panel_width  = 240;
      cfg.panel_height = 320;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.invert   = false;
      cfg.rgb_order = false;
      cfg.bus_shared = true;
      _panel.config(cfg);
    }
    { // backlight control
      auto cfg = _panel.config();
      (void)cfg;
    }
    setPanel(&_panel);

    // 背光：独立 GPIO 控制，不在面板内
  }
};
