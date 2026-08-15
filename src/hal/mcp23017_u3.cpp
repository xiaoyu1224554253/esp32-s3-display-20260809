#include "hal/mcp23017_u3.h"

#include <Arduino.h>
#include <stdint.h>

// ============================================================
// ES3C28P 无 MCP23017 扩展芯片。此文件保留接口以兼容旧代码，
// 所有函数返回"未连接/不可用"。
// ============================================================

bool mcp23017_u3_begin() {
  return false;
}

bool mcp23017_u3_is_ready() {
  return false;
}

bool mcp23017_u3_write_a(uint8_t value) {
  (void)value;
  return false;
}

bool mcp23017_u3_write_b(uint8_t value) {
  (void)value;
  return false;
}

uint8_t mcp23017_u3_read_a() {
  return 0xFF;
}

uint8_t mcp23017_u3_read_b() {
  return 0xFF;
}

bool mcp23017_u3_set_a(uint8_t bit, bool level) {
  (void)bit;
  (void)level;
  return false;
}

bool mcp23017_u3_set_b(uint8_t bit, bool level) {
  (void)bit;
  (void)level;
  return false;
}

bool mcp23017_u3_read_a_bit(uint8_t bit, bool* level) {
  (void)bit;
  if (level) *level = true;
  return false;
}

bool mcp23017_u3_read_b_bit(uint8_t bit, bool* level) {
  (void)bit;
  if (level) *level = true;
  return false;
}

void mcp23017_u3_debug_dump() {
}
