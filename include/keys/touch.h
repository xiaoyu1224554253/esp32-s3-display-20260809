#pragma once

#include <stdint.h>

// ============================================================
// FT6336G 电容触摸屏驱动（I2C，LCDWiki ES3C28P 板载）。
//
// 把底层触摸点转换为手势事件（单击 / 双击 / 长按 / 上下左右滑动），
// 供 keys.cpp 映射到现有 UI 按键语义。
// ============================================================

enum class TouchGesture : uint8_t {
    NONE = 0,
    TAP,          // 单击
    DOUBLE_TAP,   // 双击
    LONG_PRESS,   // 长按
    SWIPE_UP,
    SWIPE_DOWN,
    SWIPE_LEFT,
    SWIPE_RIGHT,
};

struct TouchEvent {
    TouchGesture gesture = TouchGesture::NONE;
    uint16_t x = 0;   // 屏幕坐标（0..239）
    uint16_t y = 0;   // 屏幕坐标（0..319）
};

// 初始化 I2C 并探测触摸芯片（失败不阻塞，后续轮询会重试）。
void touch_init();

// 是否探测到 FT6336G。
bool touch_is_present();

// 轮询触摸屏，返回最近一次手势事件；无事件返回 NONE。
// 每次调用最多返回一个事件，事件会被消费。
TouchEvent touch_poll();
