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
    // 生成特征码
    bool generateImageFeature(uint8_t bufferID);
    // 生成模版
    bool generateImageTemplate(void);
    // 存储指纹
    bool storeImageTemplate(uint16_t pos);
    // 存储指纹整体流程 1、录指纹两次，2、生成两次特征码，3、特征合成模版，4、存到某一页
    bool recordFingerprint(uint16_t pos);
    // 搜索指纹
    bool searchFingerprint(uint16_t &pos);
    // 验证指纹流程： 1、录指纹图像 2、生成特征码 3、搜索指纹
    bool verfiyFingerprint(uint16_t &pos);
    // 删除指纹
    bool deleteFingerprint(const uint16_t pos);
    // 清空指纹
    bool clearFingerprintLib(void);
    // 判断指纹是否被触摸了
    bool isTouchState(void);

private:
    LinuxUart uart;
};

#endif
