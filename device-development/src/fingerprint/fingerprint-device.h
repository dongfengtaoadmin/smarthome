#ifndef _FINGERPRINT_DEVICE_HEAD_H
#define _FINGERPRINT_DEVICE_HEAD_H

#include "linux-uart.h"
#include "fingerprint-protocol.h"

class FingerprintDevice
{
public:
    FingerprintDevice(const string &device);
    bool sendPacket(const vector<uint8_t> &packet);
    bool recvPacket(vector<uint8_t> &packet);
    // 握手
    bool handshake(void);
    // 探测手指图像
    bool detectImage(void);
    // // 生成模版
    // bool generateImageFeature(uint8_t bufferID);
    // bool generateImageTemplate(void);
    // bool storeImageTemplate(uint16_t pos);
    // bool recordFingerprint(uint16_t pos);
    // bool searchFingerprint(uint16_t &pos);
    // bool verfiyFingerprint(uint16_t &pos);
    // bool deleteFingerprint(const uint16_t pos);
    // bool clearFingerprintLib(void);

private:
    LinuxUart uart;
};

#endif
