#include <FTPAPI.h>

using namespace ELFTP;

SOCKET FTPAPI::socket_connect(char* host, int port) {
    int i = 0;
    //初始化 Socket dll
    WSADATA wsaData;
    WORD socketVersion = MAKEWORD(2, 0);
    if (WSAStartup(socketVersion, &wsaData)) {
        printf("Init socket dll error!");
        exit(1);
    }

    struct hostent* server = gethostbyname(host);
    if (!server)
        return -1;
    unsigned char ch[4];
    char ip[20];
    //一个hostname 可以对应多个ip
    while (server->h_addr_list[i] != NULL) {
        memcpy(&ch, server->h_addr_list[i], 4);
        sprintf(ip, "%d.%d.%d.%d", ch[0], ch[1], ch[2], ch[3]);
        // printf("%s\n",ip);
        i++;
    }

    //创建Socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);  // TCP socket
    if (SOCKET_ERROR == s) {
        std::cout << "Create Socket Error!" << std::endl;
        exit(1);
    }
    //设置超时连接
    int timeout = 3000;  //复杂的网络环境要设置超时判断
    int ret = setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout,
                         sizeof(timeout));
    ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout,
                     sizeof(timeout));
    //指定服务器地址
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.S_un.S_addr = inet_addr(ip);
    address.sin_port = htons((unsigned short)port);
    //连接
    if (SOCKET_ERROR == connect(s, (LPSOCKADDR)&address, sizeof(address))) {
        std::cout << "Can Not Connect To Server IP!\n";
        return 0;
    }
    return s;
}
