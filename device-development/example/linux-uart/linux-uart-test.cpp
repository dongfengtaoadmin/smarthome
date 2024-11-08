#include "linux-uart.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    LinuxUart linuxUart("/dev/ttymxc5");

    uint8_t txBuf[] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};
    int n = linuxUart.writeData(txBuf,sizeof(txBuf));

    printf("Linux uart write %d bytes : ",n);
    for(int i= 0;i < n;i ++){
        printf("%02x ",txBuf[i]);
    }
    printf("\n");

    uint8_t rxBuf[1024];
    n = linuxUart.readData(rxBuf,sizeof(rxBuf));
    printf("Linux uart read %d bytes  : ",n);

    for(int i= 0;i < n;i ++){
        printf("%02x ",rxBuf[i]);
    }
    printf("\n");

    return 0;
}
