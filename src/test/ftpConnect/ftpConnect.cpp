#include <iostream>
// #include <string>
// #include <cmath>
// #include <vector>
// #include <stdio.h>
#include <FTPAPI.h>
using namespace ELFTP;
using namespace std;
int main () {
    SOCKET s = FTPAPI::ftp_connect("209.51.188.20",21,"",""); //登录到FTP服务器
    int size;
    int ret = FTPAPI::ftp_server2local(s,"sed-2.05.bin.README","sed-2.05.bin.README",&size); //在FTP服务器获取文件
    FTPAPI::ftp_quit(s); //退出FTP服务器
    return 0;
}
