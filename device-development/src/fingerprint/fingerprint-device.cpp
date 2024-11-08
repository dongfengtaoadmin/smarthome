#include "fingerprint-device.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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

bool FingerprintDevice::clearFingerprintLib(void)
{
    FingerprintProtocol reqProtocol = FingerprintProtocol::makeClearFingerprintLibProtocol();
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

// 读取管脚电平状态 就知道是不是被触摸了
bool FingerprintDevice::isTouchState(void)
{

    // 这两种写法的主要区别在于文件名的类型和可变性：

    // 第一种写法：
    // string filename = "/sys/class/gpio/gpio134/value";
    // FILE *fp = fopen(filename.c_str(), "r");
    // 类型：filename 是一个 std::string 对象。
    // 用法：std::string 提供了许多字符串处理功能，可以方便地进行拼接、查找、替换等操作。
    // 转换：fopen 函数接受 const char* 参数，因此在使用 filename 时需要调用 .c_str() 方法将 std::string 转换为 C 风格的字符串（const char*）。
    // 优点：可以利用 std::string 类的各种方法来灵活处理字符串内容（如动态生成文件名或处理路径等），非常适合需要动态构建字符串的情况。

    // 第二种写法：

    // const char *filename = "/sys/class/gpio/gpio134/value";
    // FILE *fp = fopen(filename, "r");
    // 类型：filename 是一个 const char*，指向 C 风格的字符串。
    // 不可变性：const char* 是一个指针，指向字符串字面量（在程序的常量区存储）。这种字符串通常是只读的。
    // 直接使用：由于 filename 本身已经是 const char*，可以直接传递给 fopen，无需转换。
    // 优点：占用的内存较小，且性能稍微好一些，因为它避免了从 std::string 到 const char* 的转换。适用于文件路径是固定的情况。

    // 总结
    // 如果文件名是固定的、不需要修改，使用 const char* 更简单、直接。
    // 如果文件名需要动态生成或修改，使用 std::string 更灵活。

    const char *filename = "/sys/class/gpio/gpio134/value"; // 定义 GPIO 134 的电平状态文件的路径

    FILE *fp = fopen(filename, "r"); // 尝试以只读模式打开电平状态文件

    if (fp != nullptr) // 如果文件成功打开（fp 不为 nullptr），则跳过内部的 `if` 代码块
    {
        // 判断文件打开失败的原因是否是文件不存在（errno == ENOENT）
        if (errno == ENOENT)
        {
            system("echo 134 > /sys/class/gpio/export");           // 导出 GPIO 134，使其可以通过 /sys/class/gpio/gpio134 访问
            system("echo in > /sys/class/gpio/gpio134/direction"); // 将 GPIO 134 的方向设置为输入模式

            fp = fopen(filename, "r"); // 再次尝试以只读模式打开电平状态文件

            if (errno == ENOENT) // 如果仍然失败，并且原因是文件不存在，则输出错误信息
            {
                fprintf(stderr, "Fail to fopen %s: %s\n", filename, strerror(errno)); // 打印错误信息
                return false;                                                         // 返回 false 表示操作失败
            }
        }
    }

    // 读取 GPIO 电平状态
    int leve = fgetc(fp); // 使用 fgetc 从文件中读取一个字符，表示电平状态（'1' 为高电平，'0' 为低电平）

    fclose(fp); // 关闭文件

    return leve == '1' ? true : false; // 返回电平状态，如果是 '1' 则返回 true，否则返回 false
}
