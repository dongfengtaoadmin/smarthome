#include "fingerprint-protocol.h"

int main(int argc, char const *argv[])
{
    // 静态函数直接通过类名调用，语法简洁直观
    FingerprintProtocol protocol = FingerprintProtocol::makeHandshakeProtocol();
    protocol.showPacket();

    printf("--------------------------------------------------------\n");
    // // 根据应答的数据流包 生成 fingerprint 对象
    vector<uint8_t> handShakeAckPacket = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
                                          0x07,0x00,0x03,0x00,0x00,0x0A};
    FingerprintProtocol handshakeAckProtocol = FingerprintProtocol::fromProtocolPacket(handShakeAckPacket);
    handshakeAckProtocol.showPacket();

    cout << "handshake ack packet is error : " << handshakeAckProtocol.isPacketError() << endl;

    printf("--------------------------------------------------------\n");
    FingerprintProtocol imageDetectProtocol = FingerprintProtocol::makeDetectImageProtocol();
    imageDetectProtocol.showPacket();
    
    printf("--------------------------------------------------------\n");
    FingerprintProtocol imageFeatureProtocol = FingerprintProtocol::makeImageFeatureProtocol(0x1);
    imageFeatureProtocol.showPacket();

    printf("--------------------------------------------------------\n");
    FingerprintProtocol imageTemplateProtocol = FingerprintProtocol::makeImageTemplateProtocol();
    imageTemplateProtocol.showPacket();

    printf("--------------------------------------------------------\n");
    FingerprintProtocol storeTemplateProtocol = FingerprintProtocol::makeStoreTemplateProtocol(1);
    storeTemplateProtocol.showPacket();

    printf("--------------------------------------------------------\n");
    FingerprintProtocol searchFingerprintProtocol = FingerprintProtocol::makeSearchFingerprintProtocol();
    searchFingerprintProtocol.showPacket();

    printf("--------------------------------------------------------\n");
    FingerprintProtocol deleteFingerprintProtocol = FingerprintProtocol::makeDeleteFingerprintProtocol(1);
    deleteFingerprintProtocol.showPacket();

    printf("--------------------------------------------------------\n");
    FingerprintProtocol clearFingerprintLibProtocol = FingerprintProtocol::makeClearFingerprintLibProtocol();
    clearFingerprintLibProtocol.showPacket();
    printf("--------------------------------------------------------\n");
    vector<uint8_t> packet = clearFingerprintLibProtocol.getPacket();
    for(auto data : packet){
        printf("%02x ",data);
    }
    printf("\n");

    return 0;
}
