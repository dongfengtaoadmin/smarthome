#include "fingerprint-device.h"

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
