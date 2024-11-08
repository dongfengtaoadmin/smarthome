#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include "linux-uart.h"

LinuxUart::LinuxUart(const string &deviceName, int baudRate)
{
    fd = open(deviceName.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        fprintf(stderr, "Fail to open %s,err:%s\n", deviceName.c_str(), strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("open uart:%s success\n", deviceName.c_str());
    bool ok = defaultInit(baudRate);
    if (!ok)
    {
        fprintf(stderr, "Fail to default init Uart\n");
        exit(EXIT_FAILURE);
    }
}

LinuxUart::~LinuxUart()
{
    close(fd);
}

bool LinuxUart::defaultInit(int baudRate)
{
    int ret;
    int err;
    struct termios tio;

    bzero(&tio, sizeof(tio));
    tio.c_cflag |= CLOCAL | CREAD;
    tio.c_cflag &= ~CSIZE;

    // 设置数据位为8bit
    tio.c_cflag |= CS8;

    // 设置校验位:无奇偶校验位
    tio.c_cflag &= ~PARENB;

    // 设置波特率
    switch (baudRate)
    {
    case 4800:
        cfsetispeed(&tio, B4800);
        cfsetospeed(&tio, B4800);
        break;
    case 9600:
        cfsetispeed(&tio, B9600);
        cfsetospeed(&tio, B9600);
        break;
    case 57600:
        cfsetispeed(&tio, B57600);
        cfsetospeed(&tio, B57600);
        break;
    case 115200:
        cfsetispeed(&tio, B115200);
        cfsetospeed(&tio, B115200);
        break;
    default:
        fprintf(stderr, "The baudrate:%d is not support\n", baudRate);
        return false;
    }

    // 设置停止位为1bit
    tio.c_cflag &= ~CSTOPB;

    // 设置等待时间和最小接收字符
    tio.c_cc[VTIME] = 30;
    tio.c_cc[VMIN] = 0;

    // 刷新串口:处理未接收字符
    tcflush(fd, TCIOFLUSH);

    // 设置参数
    err = tcsetattr(fd, TCSANOW, &tio);
    if (err){
        fprintf(stderr,"Fail to tcsetattr,err:%s\n", strerror(errno));
        return false;
    }

    return true;
}

int LinuxUart::readData(uint8_t *buf, uint32_t size)
{
    int n;

    n = read(fd,buf,size);
    if(n < 0){
        fprintf(stderr,"Fail to read,err:%s\n",strerror(errno));
        return -1;
    }

    return n;
}

int LinuxUart::readFixLenData(uint8_t *buf, uint32_t fixLen)
{
    int n;
    int count = 0;
    //比如要读取的固定长度是 6 第一次读取到了 5个 那么 

    //第一次读取
    // while (count < fixLen)：因为 count 初始值是 0，满足条件，进入循环。
    // n = read(fd, buf + count, fixLen - count);
    // count = 0，所以 buf + count 还是指向 buf 的起始位置。
    // fixLen - count = 6 - 0 = 6，表示尝试读取 6 个字节。
    // 结果：read 实际读取到了 5 个字节，将它们写入 buf 的前 5 个位置。
    // if (n <= 0)：n = 5，不满足 n <= 0 的条件，不会退出循环。
    // count += n;：更新 count 的值。
    // count = 0 + 5 = 5，表示当前已成功读取了 5 个字节。

    // 第二次读取
    // 现在 count = 5，还有 fixLen - count = 6 - 5 = 1 个字节未读取，循环再次执行。
    // while (count < fixLen)：count = 5，仍小于 fixLen = 6，继续循环。
    // n = read(fd, buf + count, fixLen - count);
    // buf + count 此时指向 buf 的第 6 个位置。
    // fixLen - count = 1，所以这次只请求读取 1 个字节。
    // 结果：read 成功读取到 1 个字节，放入 buf 的第 6 个位置。
    // if (n <= 0)：n = 1，不满足退出条件。
    // count += n;
    // count = 5 + 1 = 6，表示已成功读取了 6 个字节。

    while(count < fixLen){
        n = read(fd,buf + count,fixLen - count);
        if(n <= 0){
            break;
        }

        count += n;
    }

    return count != fixLen ? -1 : fixLen;
}

int LinuxUart::writeData(const uint8_t *buf, uint32_t size)
{
    int n;

    n = write(fd,buf,size);
    if(n < 0){
        fprintf(stderr,"Fail to write,err:%s\n",strerror(errno));
        return -1;
    }

    return n;
}
