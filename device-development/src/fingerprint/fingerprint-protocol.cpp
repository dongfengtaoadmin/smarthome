#include "fingerprint-protocol.h"

// 成员初始化列表（Member Initializer List），用来在构造函数中直接初始化成员变量。
// 有参构造函数
// 成员 packet 被初始化为参数 packet 的内容
// 传引用的参数 packet 避免了不必要的拷贝，传递更高效
// packet(packet) 初始化成员变量时不用 &，初始化成员变量时写 packet(packet)，会复制参数的值到成员变量，而不是引用它 因为 一旦构造函数结束，参数 packet 不再存在，但成员变量 packet 已经独立保存了一份数据，能够在对象的整个生命周期中使用
FingerprintProtocol::FingerprintProtocol(const vector<uint8_t> &packet):packet(packet)
{
}

// 无参构造函数
FingerprintProtocol::FingerprintProtocol()
{
}



void FingerprintProtocol::addPacketHead(const uint16_t head)
{
    packet.push_back(head >> 8); // 读取高8位
    packet.push_back(head & 0xff); // 读取低 8位

}

void FingerprintProtocol::addPacketAddr(uint32_t addr)
{
    //该循环运行 4 次，因为 addr 是 32 位整数，可以分为 4 个字节（每个字节 8 位），代表地址的四个字节

    // 循环分析
    // 第一次循环 (i = 0):

    // 计算:
    // addr >> 24 结果：0xFFFFFFFF >> 24 = 0x000000FF（高 8 位为 11111111，即 255）
    // packet.push_back(0xFF) 将 0xFF 添加到 packet。
    // 更新 addr:
    // addr = 0xFFFFFFFF << 8 = 0xFFFFFF00（二进制：11111111 11111111 11111111 00000000）
    // 第二次循环 (i = 1):

    // 计算:
    // addr >> 24 结果：0xFFFFFF00 >> 24 = 0x000000FF（高 8 位为 11111111，即 255）
    // packet.push_back(0xFF) 将 0xFF 添加到 packet。
    // 更新 addr:
    // addr = 0xFFFFFF00 << 8 = 0xFFFF0000（二进制：11111111 11111111 00000000 00000000）
    // 第三次循环 (i = 2):

    // 计算:
    // addr >> 24 结果：0xFFFF0000 >> 24 = 0x000000FF（高 8 位为 11111111，即 255）
    // packet.push_back(0xFF) 将 0xFF 添加到 packet。
    // 更新 addr:
    // addr = 0xFFFF0000 << 8 = 0xFF000000（二进制：11111111 00000000 00000000 00000000）
    // 第四次循环 (i = 3):

    // 计算:
    // addr >> 24 结果：0xFF000000 >> 24 = 0x000000FF（高 8 位为 11111111，即 255）
    // packet.push_back(0xFF) 将 0xFF 添加到 packet。
    // 更新 addr:
    // addr = 0xFF000000 << 8 = 0x00000000（二进制：00000000 00000000 00000000 00000000）
    // 最终结果
    // 在函数执行完毕后，packet 中将依次添加 4 个字节，每个字节都是 0xFF。最终 packet 的内容如下：

    // packet = {0xFF, 0xFF, 0xFF, 0xFF}; // 共 4 个字节，值均为 255
    for(int i = 0;i < 4;i ++){
        packet.push_back(addr >> 24); //addr >> 24：对 addr 右移 24 位，使最高的 8 位移动到最右侧，并将低位截断 如果 addr 为 0xFFFFFFFF，右移 24 位后的结果是 0xFF。。
        addr = addr << 8; //addr << 8：左移 addr 8 位，相当于将已处理的最高字节移出，使下一个字节成为新的最高位
    }

    //packet = [0xFF, 0xFF, 0xFF, 0xFF]
}

void FingerprintProtocol::addPacketID(uint8_t pid)
{
    packet.push_back(pid);
}

void FingerprintProtocol::addPacketLen(uint16_t len)
{
    packet.push_back(len >> 8);
    packet.push_back(len & 0xff);
}

void FingerprintProtocol::addPacketContent(uint8_t *data, uint32_t size)
{
    for(int i = 0;i < size;i ++){
        packet.push_back(data[i]);
    }
}

void FingerprintProtocol::addPacketSum(void)
{
    uint16_t sum = 0;
    //这里 6 的原因是 不要前 6个字节 而是从 包标识 到 包内容 的总和 就是 校验和
    for(int i = 6;i < packet.size();i ++){
        sum += packet[i];
    }

    packet.push_back(sum >> 8);
    packet.push_back(sum & 0xff);
}

void FingerprintProtocol::showPacket(void) const
{
    packet_t *pkt = (packet_t *)packet.data();
    printf("packet head : %04x\n",getPacketHead());
    printf("packet addr : %08x\n",getPacketAddr());
    printf("packet id   : %02x\n",getPacketID());
    printf("packet len  : %04x\n",getPacketDataLen() + 2);
    printf("packet data : ");

    uint16_t dataLen = getPacketDataLen();
    uint8_t  *data   = getPacketData();
    for(int i = 0;i < dataLen;i ++){
        printf("%02x ",data[i]);
    }
    printf("\n");

    printf("packet sum  : %04x\n",getPacketSum());
}

uint16_t FingerprintProtocol::getPacketHead(void) const
{
    // 这里把 packet.data 这个数组强制转成 packet_t 结构体 之后就可以使用 ->head 进行赋值了
    packet_t *pkt = (packet_t *)packet.data();
    //左移是后面补 0 这里 腾出八位 放 
    //head 数组的两个字节对应 packet 的前两个字节。
    return (pkt->head[0] << 8 | pkt->head[1]);
}

uint32_t FingerprintProtocol::getPacketAddr(void) const
{
    packet_t *pkt = (packet_t *)packet.data();
    uint32_t packetAddr = (pkt->addr[0] << 24) | (pkt->addr[1] << 16) |
                          (pkt->addr[2] << 8 ) | (pkt->addr[3] << 0 ) ;
    return packetAddr;
}

uint8_t FingerprintProtocol::getPacketID(void) const
{
    packet_t *pkt = (packet_t *)packet.data();
    return pkt->pid;
}

// 这里获取的书数据长度 所以减 2 
uint16_t FingerprintProtocol::getPacketDataLen(void) const
{
    packet_t *pkt = (packet_t *)packet.data();
    return (pkt->len[0] << 8 | pkt->len[1]) - 2; // 减 2 的原因是 减去 两个字节的 校验和
}

uint8_t *FingerprintProtocol::getPacketData(void) const
{
    packet_t *pkt = (packet_t *)packet.data();
    return pkt->data;
}

uint16_t FingerprintProtocol::getPacketSum(void) const
{
    int packetSize = packet.size();
    return (packet[packetSize - 2] << 8 | packet[packetSize - 1]);
}

vector<uint8_t> FingerprintProtocol::getPacket(void) const
{
    return packet;
}
//  - 2 的原因是 手册上面说的 - 2 校验和 才是长度
bool FingerprintProtocol::isPacketError(void)
{
    uint16_t calcSum = 0;

    for(int i = 6;i < packet.size() - 2;i ++){
        calcSum += packet[i];
    }

    uint16_t packetSum = getPacketSum();
    printf("CalcSum:0x%04x,PacketSum:0x%04x\n",calcSum,packetSum);

    return calcSum == packetSum ? false : true;
}

FingerprintProtocol FingerprintProtocol::makeHandshakeProtocol(void)
{
    FingerprintProtocol protocol;

    protocol.addPacketHead();
    protocol.addPacketAddr();
    protocol.addPacketID(COMMAND_PACKET);
    protocol.addPacketLen(0x03);
    uint8_t data[] = {0x53};
    protocol.addPacketContent(data,sizeof(data));
    protocol.addPacketSum();

    return protocol;
}

// 收到一组数据后直接放到   Fingerprint 中
FingerprintProtocol FingerprintProtocol::fromProtocolPacket(const vector<uint8_t> &packet)
{
    return FingerprintProtocol(packet);
}

FingerprintProtocol FingerprintProtocol::makeDetectImageProtocol(void)
{
    FingerprintProtocol protocol;

    protocol.addPacketHead();
    protocol.addPacketAddr();
    protocol.addPacketID(COMMAND_PACKET);
    protocol.addPacketLen(0x03);
    uint8_t data[] = {0x01};
    protocol.addPacketContent(data,sizeof(data));
    protocol.addPacketSum();

    return protocol;
}

/**
bufferID 缓冲器 id  01 或者 02 
 */
FingerprintProtocol FingerprintProtocol::makeImageFeatureProtocol(uint8_t bufferID)
{
    FingerprintProtocol protocol;

    protocol.addPacketHead();
    protocol.addPacketAddr();
    protocol.addPacketID(COMMAND_PACKET);
    protocol.addPacketLen(0x04);
    uint8_t data[] = {0x02,bufferID};
    protocol.addPacketContent(data,sizeof(data));
    protocol.addPacketSum();

    return protocol;
}

/**
 * 生成模版
 */
FingerprintProtocol FingerprintProtocol::makeImageTemplateProtocol(void)
{
    FingerprintProtocol protocol;

    protocol.addPacketHead();
    protocol.addPacketAddr();
    protocol.addPacketID(COMMAND_PACKET);
    protocol.addPacketLen(0x03);
    uint8_t data[] = {0x05};
    protocol.addPacketContent(data,sizeof(data));
    protocol.addPacketSum();

    return protocol;
}

/**
pos 存储模版的位置 0 - 49
虽然 1 是一个 int 类型的常量，但它会隐式转换为 uint16_t，最终只会占用 2 个字节
 */
FingerprintProtocol FingerprintProtocol::makeStoreTemplateProtocol(uint16_t pos)
{
    FingerprintProtocol protocol;

    protocol.addPacketHead();
    protocol.addPacketAddr();
    protocol.addPacketID(COMMAND_PACKET);
    protocol.addPacketLen(0x06);
    // 假设 pos = 1，则 pos 的二进制表示为 0000 0000 0000 0001（16 位），只有最低位是 1，其余位为 0
    // (uint8_t)(pos >> 8) 得到 0x00。
    // (uint8_t)(pos & 0xff) 得到 0x01。
    uint8_t data[] = {0x06,0x01,(uint8_t)(pos >> 8),(uint8_t)(pos & 0xff)};
    protocol.addPacketContent(data,sizeof(data));
    protocol.addPacketSum();

    return protocol;
}

FingerprintProtocol FingerprintProtocol::makeSearchFingerprintProtocol(uint16_t startPos,
                                                                       uint16_t n)
{
    FingerprintProtocol protocol;

    protocol.addPacketHead();
    protocol.addPacketAddr();
    protocol.addPacketID(COMMAND_PACKET);
    protocol.addPacketLen(0x08);
    uint8_t data[] = {0x04,0x01,(uint8_t)(startPos >> 8),(uint8_t)(startPos & 0xff),
                      (uint8_t)(n >> 8),(uint8_t)(n & 0xff)};
    protocol.addPacketContent(data,sizeof(data));
    protocol.addPacketSum();

    return protocol;
}

FingerprintProtocol FingerprintProtocol::makeDeleteFingerprintProtocol(uint16_t pos)
{
    FingerprintProtocol protocol;

    protocol.addPacketHead();
    protocol.addPacketAddr();
    protocol.addPacketID(COMMAND_PACKET);
    protocol.addPacketLen(0x07);
    
    //删除的区间
    //0x00,0x01 表示要删除的个数这里 是1个
    uint8_t data[] = {0x0c,(uint8_t)(pos >> 8),(uint8_t)(pos & 0xff),0x00,0x01};
    protocol.addPacketContent(data,sizeof(data));
    protocol.addPacketSum();

    return protocol;
}

FingerprintProtocol FingerprintProtocol::makeClearFingerprintLibProtocol(void)
{
    FingerprintProtocol protocol;

    protocol.addPacketHead();
    protocol.addPacketAddr();
    protocol.addPacketID(COMMAND_PACKET);
    protocol.addPacketLen(0x03);
    uint8_t data[] = {0x0d};
    protocol.addPacketContent(data,sizeof(data));
    protocol.addPacketSum();

    return protocol;
}

