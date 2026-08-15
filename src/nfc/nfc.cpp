#include <Arduino.h>

#include "nfc/nfc.h"

// ============================================================
// ES3C28P 无 RC522/NFC 读卡硬件。此文件保留 nfc.h 全部接口以兼容
// 上层（NFC 管理页、绑定逻辑等），但所有硬件相关操作返回"未连接"。
// NFC 绑定数据层（nfc_binding.cpp，纯 TF 文件读写）不受影响。
// ============================================================

void nfc_init(void)
{
    Serial.println("[NFC] ES3C28P 无 RC522，NFC 硬件已禁用");
}

void nfc_poll(void)
{
    // 无硬件，不轮询
}

bool nfc_take_last_uid(String& out_uid)
{
    (void)out_uid;
    return false;
}

bool nfc_is_uid_present(const String& uid)
{
    (void)uid;
    return false;
}

void nfc_ignore_uid_once(const String& uid, uint32_t ms)
{
    (void)uid;
    (void)ms;
}

bool nfc_take_last_card_info(String& out_uid, String& out_card_type)
{
    (void)out_uid;
    (void)out_card_type;
    return false;
}
