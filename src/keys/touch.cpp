#include <Arduino.h>
#include <Wire.h>

#include "keys/touch.h"
#include "board/board_pins.h"
#include "utils/log.h"

// ============================================================
// FT6336G 触摸驱动。
//
// 寄存器（FT6x36 系）：
//   0x02      触摸点数
//   0x03/0x04 触摸1 X 高4位/低8位（12bit）
//   0x05/0x06 触摸1 Y 高4位/低8位（12bit）
//   0x15      Chip ID
//   0xA8      Firmware 版本
// ============================================================

static constexpr uint8_t FT_I2C_ADDR = 0x38;

// ---- 坐标轴方向（若屏幕画面与触摸方向不一致，改这里即可） ----
static constexpr bool TOUCH_SWAP_XY   = false;   // 交换 X/Y
static constexpr bool TOUCH_INVERT_X  = false;   // 反转 X
static constexpr bool TOUCH_INVERT_Y  = false;   // 反转 Y

static constexpr uint16_t SCR_W = 240;
static constexpr uint16_t SCR_H = 320;

// ---- 手势判定阈值 ----
static constexpr uint16_t SWIPE_THRESHOLD    = 40;   // 滑动判定距离(px)
static constexpr uint32_t LONG_PRESS_MS      = 600;  // 长按时长
static constexpr uint32_t DOUBLE_TAP_WINDOW  = 300;  // 双击判定窗口
static constexpr uint32_t MIN_TAP_MS         = 25;   // 过快的误触忽略
static constexpr uint16_t JITTER_GUARD       = 6;    // 抖动忽略距离

// ---- 状态 ----
static bool       s_present    = false;
static bool       s_probed     = false;
static uint32_t   s_probe_t0   = 0;
static bool       s_down       = false;
static uint32_t   s_down_t0    = 0;
static int16_t    s_down_x     = 0;
static int16_t    s_down_y     = 0;
static int16_t    s_cur_x      = 0;
static int16_t    s_cur_y      = 0;
static int16_t    s_max_dx     = 0;
static int16_t    s_max_dy     = 0;
static int16_t    s_final_dx   = 0;
static int16_t    s_final_dy   = 0;
static bool       s_long_fired = false;
static uint32_t   s_last_tap_ms = 0;

static bool read_regs(uint8_t reg, uint8_t* buf, uint8_t len)
{
    Wire.beginTransmission(FT_I2C_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        return false;
    }
    if (Wire.requestFrom((int)FT_I2C_ADDR, (int)len) != len) {
        return false;
    }
    for (uint8_t i = 0; i < len; ++i) {
        buf[i] = Wire.read();
    }
    return true;
}

// 探测芯片：读取 Chip ID / Firmware。失败会在后续轮询中周期性重试。
static void probe_chip()
{
    uint8_t id = 0, fw = 0;
    bool ok = read_regs(0x15, &id, 1) && read_regs(0xA8, &fw, 1);
    if (ok) {
        s_present = true;
        LOGI("[触摸] FT6336G 探测成功 ChipID=0x%02X FW=0x%02X", id, fw);
    } else {
        s_present = false;
        LOGW("[触摸] FT6336G 未响应（将重试）");
    }
    s_probed = true;
    s_probe_t0 = millis();
}

// 读取单个触摸点（返回是否正在触摸，并给出归一化屏幕坐标）。
static bool read_touch(int16_t& out_x, int16_t& out_y)
{
    uint8_t buf[6];
    if (!read_regs(0x02, buf, sizeof(buf))) {
        return false;
    }

    uint8_t num = buf[0] & 0x07;
    if (num == 0) {
        return false;
    }

    // 触摸1：X=(buf[1]&0x0F)<<8|buf[2]，Y=(buf[3]&0x0F)<<8|buf[4]
    int16_t rx = (int16_t)(((buf[1] & 0x0F) << 8) | buf[2]);
    int16_t ry = (int16_t)(((buf[3] & 0x0F) << 8) | buf[4]);

    // 归一化到 240x320（不同模块原始分辨率可能有差异）
    uint16_t nx = 0, ny = 0;
    if (rx > 0) nx = (uint16_t)((uint32_t)rx * SCR_W / 4096);
    if (ry > 0) ny = (uint16_t)((uint32_t)ry * SCR_H / 4096);
    if (nx >= SCR_W) nx = SCR_W - 1;
    if (ny >= SCR_H) ny = SCR_H - 1;

    int16_t x = (int16_t)nx;
    int16_t y = (int16_t)ny;

    if (TOUCH_SWAP_XY) {
        int16_t t = x; x = y; y = t;
    }
    if (TOUCH_INVERT_X) x = (int16_t)(SCR_W - 1 - x);
    if (TOUCH_INVERT_Y) y = (int16_t)(SCR_H - 1 - y);

    out_x = x;
    out_y = y;
    return true;
}

void touch_init()
{
    Wire.begin(board::PIN_TOUCH_SDA, board::PIN_TOUCH_SCL);
    Wire.setClock(400000);
    probe_chip();
}

bool touch_is_present()
{
    return s_present;
}

TouchEvent touch_poll()
{
    TouchEvent ev;

    // 探测失败时周期性重试（每 2 秒一次）。
    if (!s_present && (!s_probed || (millis() - s_probe_t0) >= 2000)) {
        probe_chip();
    }
    if (!s_present) {
        return ev;
    }

    int16_t x = 0, y = 0;
    const bool down = read_touch(x, y);
    const uint32_t now = millis();

    if (down && !s_down) {
        // 新按下
        s_down = true;
        s_down_t0 = now;
        s_down_x = x;
        s_down_y = y;
        s_cur_x = x;
        s_cur_y = y;
        s_max_dx = 0;
        s_max_dy = 0;
        s_final_dx = 0;
        s_final_dy = 0;
        s_long_fired = false;
    } else if (down && s_down) {
        // 持续按住：记录位移
        int16_t dx = (int16_t)(x - s_down_x);
        int16_t dy = (int16_t)(y - s_down_y);
        int16_t adx = dx < 0 ? (int16_t)-dx : dx;
        int16_t ady = dy < 0 ? (int16_t)-dy : dy;
        // 抖动忽略：位移累计到超过阈值才更新基准点
        if (adx > JITTER_GUARD || ady > JITTER_GUARD) {
            s_cur_x = x;
            s_cur_y = y;
            if (adx > s_max_dx) s_max_dx = adx;
            if (ady > s_max_dy) s_max_dy = ady;
            s_final_dx = dx;
            s_final_dy = dy;
        }

        // 长按判定（位移小且按住超时）
        if (!s_long_fired &&
            (now - s_down_t0) >= LONG_PRESS_MS &&
            s_max_dx < SWIPE_THRESHOLD && s_max_dy < SWIPE_THRESHOLD) {
            s_long_fired = true;
            ev.gesture = TouchGesture::LONG_PRESS;
            ev.x = (uint16_t)s_cur_x;
            ev.y = (uint16_t)s_cur_y;
            return ev;
        }
    } else if (!down && s_down) {
        // 松开
        s_down = false;
        const uint32_t dur = now - s_down_t0;

        if (s_long_fired) {
            // 长按已在按住期间上报，松开不再重复
            return ev;
        }

        if (dur < MIN_TAP_MS) {
            // 误触
            return ev;
        }

        // 滑动判定
        int16_t adx = s_final_dx < 0 ? (int16_t)-s_final_dx : s_final_dx;
        int16_t ady = s_final_dy < 0 ? (int16_t)-s_final_dy : s_final_dy;
        if (adx >= SWIPE_THRESHOLD || ady >= SWIPE_THRESHOLD) {
            if (adx > ady) {
                ev.gesture = (s_final_dx > 0) ? TouchGesture::SWIPE_RIGHT
                                              : TouchGesture::SWIPE_LEFT;
            } else {
                ev.gesture = (s_final_dy > 0) ? TouchGesture::SWIPE_DOWN
                                              : TouchGesture::SWIPE_UP;
            }
            ev.x = (uint16_t)s_cur_x;
            ev.y = (uint16_t)s_cur_y;
            return ev;
        }

        // 单击 / 双击
        if (s_last_tap_ms != 0 && (now - s_last_tap_ms) <= DOUBLE_TAP_WINDOW) {
            s_last_tap_ms = 0;
            ev.gesture = TouchGesture::DOUBLE_TAP;
        } else {
            s_last_tap_ms = now;
            ev.gesture = TouchGesture::TAP;
        }
        ev.x = (uint16_t)s_cur_x;
        ev.y = (uint16_t)s_cur_y;
        return ev;
    }

    return ev;
}
