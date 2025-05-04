#ifndef UDPSEND_HPP
#define UDPSEND_HPP
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define CH_COUNT 12  // 数据通道数量

// 定义结构体并赋初值
struct VofaData {
    VofaData() : fdata{}, tail{0x00, 0x00, 0x80, 0x7f} {}
    float fdata[CH_COUNT];
    unsigned char tail[4];
};

class UDPSender {
public:
    UDPSender() : sockfd(-1) {}

    virtual ~UDPSender() {
        if (sockfd != -1)
            close(sockfd);
    }

    bool init(const char* ip, int port) {
        // 创建UDP套接字
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1) {
            std::cerr << "Error: Failed to create UDP socket." << std::endl;
            return false;
        }
 
        // 设置目标地址信息
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);

        return true;
    }

    void send_data(const VofaData &vofa_data) {
        if (sockfd == -1) {
            std::cerr << "Error: UDP socket not initialized." << std::endl;
            return;
        }

        // 发送数据
        ssize_t ret = sendto(sockfd, (char *)vofa_data.fdata, sizeof(vofa_data.fdata), 0, (struct sockaddr*)&addr, sizeof(addr));
        sendto(sockfd, (char *)vofa_data.tail, sizeof(vofa_data.tail), 0, (struct sockaddr*)&addr, sizeof(addr));

        if (ret == -1) {
            std::cerr << "Error: Failed to send data." << std::endl;
            return;
        }
    }

private:
    int sockfd; // 套接字描述符
    struct sockaddr_in addr; // 目标地址结构体
};

// int main() {
//     UDPSender sender;
//     if (!sender.init("127.0.0.1", 5005)) {
//         std::cerr << "Error: Failed to initialize UDP sender." << std::endl;
//         return EXIT_FAILURE;
//     }

//     VofaData vofa_data;
//     // 按需填充vofa_data数据
//     vofa_data.fdata[0] = 1;
//     vofa_data.fdata[1] = 2;
//     vofa_data.fdata[2] = 3;
//     vofa_data.fdata[3] = 4;
//     sender.send_data(vofa_data);  // 发送数据

//     return EXIT_SUCCESS;
// }


#endif 