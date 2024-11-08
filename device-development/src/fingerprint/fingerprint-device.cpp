#include "fingerprint-device.h"
#include <unistd.h>

FingerprintDevice::FingerprintDevice(const string &device) : uart(device, 57600)
{
}

bool FingerprintDevice::sendPacket(const vector<uint8_t> &packet)
{
    return uart.writeData(packet.data(), packet.size()) < 0 ? false : true;
}

/*
无法知道指纹模块返回的数据是多少 所以要固定接受两个
*/
bool FingerprintDevice::recvPacket(vector<uint8_t> &packet)
{
    vector<uint8_t> packet1(PACKET_FIX_LEN); // 创建 vector packet1，用于存储固定长度的部分数据
    // 如果 PACKET_FIX_LEN 是 6 则 packet1.size() 就是 6
    bool ok = uart.readFixLenData(packet1.data(), packet1.size()); // 读取固定长度的数据，并存储在 packet1 中
    if (!ok)
    {                                                // 如果读取失败
        fprintf(stderr, "Fail to readFixLenData\n"); // 输出错误信息
        return false;                                // 返回 false，表示接收失败
    }

    // 将 packet1 中的固定长度数据转换为 FingerprintProtocol 实例
    FingerprintProtocol protocol = FingerprintProtocol::fromProtocolPacket(packet1);
    fprintf(stderr, "dft start \n");
    protocol.showPacket();
    fprintf(stderr, "dft end \n");
    // 创建 packet2，用于存储剩余的数据，大小为协议中数据长度字段（getPacketDataLen）+ 2
    vector<uint8_t> packet2(protocol.getPacketDataLen() + 2);

    // 读取协议剩余的数据部分并存储在 packet2 中
    ok = uart.readFixLenData(packet2.data(), packet2.size());
    if (!ok)
    {                                                // 如果读取失败
        fprintf(stderr, "Fail to readFixLenData\n"); // 输出错误信息
        return false;                                // 返回 false，表示接收失败
    }

    // 把 packet1 和 packet2 中的数据拼接到 packet 中，形成完整的数据帧
    packet.insert(packet.end(), packet1.begin(), packet1.end());
    packet.insert(packet.end(), packet2.begin(), packet2.end());

    return true; // 返回 true，表示接收成功
}

bool FingerprintDevice::handshake(void)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeHandshakeProtocol();
    bool ok = sendPacket(reqProtocol.getPacket());
    if (!ok)
    {
        fprintf(stderr, "Fail to sendPacket\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------send packet-------------\n");
    reqProtocol.showPacket();
#endif
    vector<uint8_t> ackPacket;
    ok = recvPacket(ackPacket);
    if (!ok)
    {
        fprintf(stderr, "Fail to recvPacket\n");
        return false;
    }

    FingerprintProtocol ackProtocol = FingerprintProtocol::fromProtocolPacket(ackPacket);
    if (ackProtocol.isPacketError())
    {
        fprintf(stderr, "recv packet is error\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------recv packet-------------\n");
    ackProtocol.showPacket();
#endif
    // 验证数据位是不是  0
    return ackProtocol.getPacketData()[0] ? false : true;
}

bool FingerprintDevice::detectImage(void)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeDetectImageProtocol();
    bool ok = sendPacket(reqProtocol.getPacket());
    if (!ok)
    {
        fprintf(stderr, "Fail to sendPacket\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------send packet-------------\n");
    reqProtocol.showPacket();
#endif
    vector<uint8_t> ackPacket;
    ok = recvPacket(ackPacket);
    if (!ok)
    {
        fprintf(stderr, "Fail to recvPacket\n");
        return false;
    }

    FingerprintProtocol ackProtocol = FingerprintProtocol::fromProtocolPacket(ackPacket);
    if (ackProtocol.isPacketError())
    {
        fprintf(stderr, "recv packet is error\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------recv packet-------------\n");
    ackProtocol.showPacket();
#endif
    // 验证数据位是不是  0
    return ackProtocol.getPacketData()[0] ? false : true;
}

bool FingerprintDevice::generateImageFeature(uint8_t bufferID)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeImageFeatureProtocol(bufferID);
    bool ok = sendPacket(reqProtocol.getPacket());
    if (!ok)
    {
        fprintf(stderr, "Fail to sendPacket\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------send packet-------------\n");
    reqProtocol.showPacket();
#endif

    vector<uint8_t> ackPacket;
    ok = recvPacket(ackPacket);
    if (!ok)
    {
        fprintf(stderr, "Fail to recvPacket\n");
        return false;
    }

    FingerprintProtocol ackProtocol = FingerprintProtocol::fromProtocolPacket(ackPacket);
    if (ackProtocol.isPacketError())
    {
        fprintf(stderr, "recv packet is error\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------recv packet-------------\n");
    ackProtocol.showPacket();
#endif

    return ackProtocol.getPacketData()[0] ? false : true;
}

bool FingerprintDevice::generateImageTemplate(void)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeImageTemplateProtocol();
    bool ok = sendPacket(reqProtocol.getPacket());
    if (!ok)
    {
        fprintf(stderr, "Fail to sendPacket\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------send packet-------------\n");
    reqProtocol.showPacket();
#endif

    vector<uint8_t> ackPacket;
    ok = recvPacket(ackPacket);
    if (!ok)
    {
        fprintf(stderr, "Fail to recvPacket\n");
        return false;
    }

    FingerprintProtocol ackProtocol = FingerprintProtocol::fromProtocolPacket(ackPacket);
    if (ackProtocol.isPacketError())
    {
        fprintf(stderr, "recv packet is error\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------recv packet-------------\n");
    ackProtocol.showPacket();
#endif

    return ackProtocol.getPacketData()[0] ? false : true;
}

/**
 * 存储指纹
 */
bool FingerprintDevice::storeImageTemplate(uint16_t pos)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeStoreTemplateProtocol(pos);
    bool ok = sendPacket(reqProtocol.getPacket());
    if (!ok)
    {
        fprintf(stderr, "Fail to sendPacket\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------send packet-------------\n");
    reqProtocol.showPacket();
#endif

    vector<uint8_t> ackPacket;
    ok = recvPacket(ackPacket);
    if (!ok)
    {
        fprintf(stderr, "Fail to recvPacket\n");
        return false;
    }

    FingerprintProtocol ackProtocol = FingerprintProtocol::fromProtocolPacket(ackPacket);
    if (ackProtocol.isPacketError())
    {
        fprintf(stderr, "recv packet is error\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------recv packet-------------\n");
    ackProtocol.showPacket();
#endif

    return ackProtocol.getPacketData()[0] ? false : true;
}

/**
 * 存储指纹整体流程 1、录指纹两次，2、生成两次特征码，3、特征合成模版，4、存到某一页
 */
bool FingerprintDevice::recordFingerprint(uint16_t pos)
{
    // 先第1次探测验证是否成功
    bool ok = false;
    for (int i = 0; i < 3; i++)
    {
        ok = detectImage();
        if (ok)
        {
            break;
        }
        sleep(1); // 每次给的时间 是1秒
    }
    // 把特征码 存放在  buffer1
    ok = generateImageFeature(0x1);
    if (!ok)
    {
        return false;
    }

    // 先第2次探测验证是否成功
    bool ok = false;
    for (int i = 0; i < 3; i++)
    {
        ok = detectImage();
        if (ok)
        {
            break;
        }
        sleep(1); // 每次给的时间 是1秒
    }
    // 把特征码 存放在  buffer2
    ok = generateImageFeature(0x2);
    if (!ok)
    {
        return false;
    }
    // 特征合成模版
    ok = generateImageTemplate();
    if (!ok)
    {
        return false;
    }
    return storeImageTemplate(pos);

    return false;
}

bool FingerprintDevice::searchFingerprint(uint16_t &pos)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeSearchFingerprintProtocol();
    bool ok = sendPacket(reqProtocol.getPacket());
    if (!ok)
    {
        fprintf(stderr, "Fail to sendPacket\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------send packet-------------\n");
    reqProtocol.showPacket();
#endif

    vector<uint8_t> ackPacket;
    ok = recvPacket(ackPacket);
    if (!ok)
    {
        fprintf(stderr, "Fail to recvPacket\n");
        return false;
    }

    FingerprintProtocol ackProtocol = FingerprintProtocol::fromProtocolPacket(ackPacket);
    if (ackProtocol.isPacketError())
    {
        fprintf(stderr, "recv packet is error\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------recv packet-------------\n");
    ackProtocol.showPacket();
#endif

    uint8_t *data = ackProtocol.getPacketData();
    if (data[0] != 0)
    {
        return false;
    }
    // 合并成一个 两个字节的 页码给 pos 指针
    pos = (data[1] << 8) | data[2];

    return true;
}

// 验证指纹流程： 1、录指纹图像 2、生成特征码 3、搜索指纹
bool FingerprintDevice::verfiyFingerprint(uint16_t &pos)
{
    // 1、录指纹图像
    bool ok = false;
    for (int i = 0; i < 3; i++)
    {
        ok = detectImage();
        if (ok)
        {
            break;
        }
        sleep(1); //
    }

    // 2、生成特征码
    ok = generateImageFeature(0x1);
    if (!ok)
    {
        return false;
    }
    // 3、搜索指纹
    return searchFingerprint(pos);
}

/**
 * 要删除的模版号
 */
bool FingerprintDevice::deleteFingerprint(const uint16_t pos)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeDeleteFingerprintProtocol(pos);
    bool ok = sendPacket(reqProtocol.getPacket());
    if(!ok){
        fprintf(stderr,"Fail to sendPacket\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------send packet-------------\n");
    reqProtocol.showPacket();
#endif 

    vector<uint8_t> ackPacket;
    ok = recvPacket(ackPacket);
    if(!ok){
        fprintf(stderr,"Fail to recvPacket\n");
        return false;
    }    
    
    FingerprintProtocol ackProtocol = FingerprintProtocol::fromProtocolPacket(ackPacket);
    if(ackProtocol.isPacketError()){
        fprintf(stderr,"recv packet is error\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------recv packet-------------\n");
    ackProtocol.showPacket();
#endif 

    return ackProtocol.getPacketData()[0] ? false:true;
}

bool FingerprintDevice::clearFingerprintLib(void)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeClearFingerprintLibProtocol();
    bool ok = sendPacket(reqProtocol.getPacket());
    if(!ok){
        fprintf(stderr,"Fail to sendPacket\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------send packet-------------\n");
    reqProtocol.showPacket();
#endif 

    vector<uint8_t> ackPacket;
    ok = recvPacket(ackPacket);
    if(!ok){
        fprintf(stderr,"Fail to recvPacket\n");
        return false;
    }    
    
    FingerprintProtocol ackProtocol = FingerprintProtocol::fromProtocolPacket(ackPacket);
    if(ackProtocol.isPacketError()){
        fprintf(stderr,"recv packet is error\n");
        return false;
    }

#ifdef FINGERPRINT_DEVICE_DEBUG
    printf("----------recv packet-------------\n");
    ackProtocol.showPacket();
#endif 

    return ackProtocol.getPacketData()[0] ? false:true;
}
