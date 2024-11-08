#ifndef _FINGERPRINT_PROTOCOL_HEAD_H
#define _FINGERPRINT_PROTOCOL_HEAD_H
#include <vector>
#include <iostream>
#include <stdint.h>
#include <stdio.h>

using namespace std;
#define PACKET_FIX_LEN 9 
class FingerprintProtocol
{
public:
    typedef struct{
        uint8_t head[2];
        uint8_t addr[4];
        uint8_t pid;
        uint8_t len[2];
        uint8_t data[1];
    }packet_t;

    enum{
        COMMAND_PACKET = 0x01,
        DATA_PACKET    = 0x02,
        ACK_PACKET     = 0x07,
        END_PACKET     = 0x08,
    };
    
    FingerprintProtocol(const vector<uint8_t> &packet); // 有参的构造函数
    FingerprintProtocol(); // 无参的构造函数
    void addPacketHead(const uint16_t head = 0xef01);
    void addPacketAddr(uint32_t addr = 0xFFFFFFFF);
    void addPacketID(uint8_t pid);
    void addPacketLen(uint16_t len);
    void addPacketContent(uint8_t *data,uint32_t size);
    void addPacketSum(void);
    void showPacket(void) const;
    uint16_t getPacketHead(void) const;
    uint32_t getPacketAddr(void) const;
    uint8_t  getPacketID(void)   const;
    uint16_t getPacketDataLen(void)  const;
    // uint8_t * 表示这个函数返回一个指向 uint8_t 类型数据的指针 - getPacketData 函数返回的是一个指向 uint8_t 数据（通常是字节数据）的地址
    uint8_t *getPacketData(void) const;
    uint16_t getPacketSum(void) const;
    vector<uint8_t> getPacket(void) const;
    bool isPacketError(void);
    
    // static 不需要实例化类，就可以直接调用静态函数，减少了不必要的对象创建开销。
    // 静态方法可以访问类的静态成员变量或常量，因此如果需要在多个实例之间共享数据
    static FingerprintProtocol makeHandshakeProtocol(void);
    // 传引用 &packet，则直接引用原始数据，避免了不必要的拷贝，提高了效率
    // 收到一组数据后直接放到   Fingerprint 中
    static FingerprintProtocol fromProtocolPacket(const vector<uint8_t> &packet);
    //录入指纹
    static FingerprintProtocol makeDetectImageProtocol(void);
    //特征码
    static FingerprintProtocol makeImageFeatureProtocol(uint8_t bufferID);
    //合成模板
    static FingerprintProtocol makeImageTemplateProtocol(void);
    //存储模板
    static FingerprintProtocol makeStoreTemplateProtocol(uint16_t pos);
    //搜索
    static FingerprintProtocol makeSearchFingerprintProtocol(uint16_t startPos = 0,
                                                             uint16_t n = 50);
    //删除                                                         
    static FingerprintProtocol makeDeleteFingerprintProtocol(uint16_t pos);
    static FingerprintProtocol makeClearFingerprintLibProtocol(void);
private:
    vector<uint8_t> packet;
};

#endif

