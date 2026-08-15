#include "hal/board_hw_control.h"

#include <Arduino.h>
#include <driver/gpio.h>

#include "board/board_pins.h"
#include "board/board_pins_pcb1_mcp23017.h"
#include "hal/mcp23017_u3.h"
#include "utils/log.h"

// ============================================================
// LCDWiki ES3C28P 硬件控制适配层。
//
// 与 PCB1 MCP23017 版的差异：
// - 无 MCP23017 / 电池 / 充电管理 / 蓝牙 / 功放 / 电磁铁
// - 背光：GPIO45 高电平点亮
// - 音频使能：GPIO1 由 audio_i2s 控制
// 这里保留接口，缺失外设返回"未连接/默认值"，确保上层代码可编译运行。
// ============================================================

namespace {

bool s_ready = false;
bool s_bt_power_enabled = false;
bool s_bt_wakeup_enabled = false;
bool s_bt_switch_level = true;
bool s_amp_mute_enabled = true;
bool s_amp_shutdown_enabled = true;
bool s_backlight_enabled = true;

}  // namespace

bool board_hw_control_begin()
{
    // 背光默认点亮（GPIO45 高电平）。
    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL, HIGH);
    s_backlight_enabled = true;

    s_ready = true;

    LOGI("[硬件控制] ES3C28P 初始化成功 背光=GPIO%d", PIN_TFT_BL);
    return s_ready;
}

BatterySample board_hw_read_battery()
{
    // ES3C28P 无电池分压采样。
    BatterySample s{};
    return s;
}

ChargerStatus board_hw_read_charger_status()
{
    ChargerStatus s{};
    return s;
}

void board_hw_battery_status_tick()
{
}

BatteryUiStatus board_hw_get_battery_status_cached()
{
    BatteryUiStatus s{};
    return s;
}

bool board_hw_set_bt_power(bool enabled)
{
    s_bt_power_enabled = enabled;
    return false;  // 无蓝牙
}

bool board_hw_get_bt_power()
{
    return s_bt_power_enabled;
}

bool board_hw_set_bt_wakeup(bool enabled)
{
    s_bt_wakeup_enabled = enabled;
    return false;
}

bool board_hw_get_bt_wakeup()
{
    return s_bt_wakeup_enabled;
}

bool board_hw_set_bt_switch(bool level)
{
    s_bt_switch_level = level;
    return false;
}

bool board_hw_get_bt_switch()
{
    return s_bt_switch_level;
}

bool board_hw_set_backlight(bool enabled)
{
    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL, enabled ? HIGH : LOW);
    s_backlight_enabled = enabled;
    LOGI("[硬件控制] 背光 %s", enabled ? "开启" : "关闭");
    return true;
}

bool board_hw_get_backlight()
{
    return s_backlight_enabled;
}

void board_hw_power_off()
{
    LOGI("[硬件控制] ES3C28P 无电源保持脚，关机仅关闭背光");
    board_hw_set_backlight(false);
}

bool board_hw_pulse_bt_switch(uint32_t pulse_ms)
{
    (void)pulse_ms;
    return false;
}

bool board_hw_set_amp_mute(bool enabled)
{
    s_amp_mute_enabled = enabled;
    return false;  // 无独立功放控制
}

bool board_hw_get_amp_mute()
{
    return s_amp_mute_enabled;
}

bool board_hw_set_amp_shutdown(bool enabled)
{
    s_amp_shutdown_enabled = enabled;
    return false;
}

bool board_hw_get_amp_shutdown()
{
    return s_amp_shutdown_enabled;
}

// ---- 电磁铁（TC118S）无硬件，全部不可用 ----

bool board_hw_solenoid_begin()
{
    return false;
}

bool board_hw_solenoid_stop()
{
    return false;
}

bool board_hw_solenoid_pulse_a(uint32_t pulse_ms)
{
    (void)pulse_ms;
    return false;
}

bool board_hw_solenoid_pulse_b(uint32_t pulse_ms)
{
    (void)pulse_ms;
    return false;
}

bool board_hw_solenoid_flip(uint32_t pulse_ms)
{
    (void)pulse_ms;
    return false;
}

void board_hw_solenoid_tick()
{
}

bool board_hw_solenoid_is_busy()
{
    return false;
}

void board_hw_debug_dump()
{
    LOGD("[硬件控制] 背光=%d", s_backlight_enabled ? 1 : 0);
}
