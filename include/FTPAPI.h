#pragma once

#include <winsock2.h>

#include <iostream>
#pragma comment(lib, "ws2_32.lib")

namespace ELFTP {

class FTPAPI {
    //成功
    static const int FTP_SUCCESS = 200;
    //服务器就绪
    static const int FTP_SERVICE_READY = 220;
    //登录因特网服务器
    static const int FTP_LOGIN_SUCCESS = 230;
    //文件行为完成
    static const int FTP_FILE_ACTION_COMPLETE = 250;
    //文件创建成功
    static const int FTP_FILE_CREATED = 257;
    //要求密码
    static const int FTP_PASSWORD_REQUIREd = 331;
    //用户密码错误
    static const int FTP_LOGIN_PASSWORD_INCORRECT = 530;

   public:
    static SOCKET socket_connect(char* host, int port);
    static SOCKET connect_server(char* host, int port);
    static int ftp_sendcmd_re(SOCKET sock,
                              char* cmd,
                              char* re_buf,
                              signed long* len);
    static int ftp_sendcmd(SOCKET sock, char* cmd);
    static int login_server(SOCKET sock, char* user, char* pwd);
    static void socket_close(int c_sock);

    /* 可用命令 */
    //连接到服务器
    static SOCKET ftp_connect(char* host, int port, char* user, char* pwd);
    //断开连接
    static int ftp_quit(SOCKET sock);
    //设置FTP传输类型
    static int ftp_type(SOCKET sock, char mode);
    //更改工作目录
    static int ftp_cwd(SOCKET sock, char* path);
    //回到上级目录
    static int ftp_cdup(SOCKET sock);
    //创建目录
    static int ftp_mkd(SOCKET sock, char* path);
    //连接到PASV接口
    static SOCKET ftp_pasv_connect(SOCKET c_sock);
    //列出FTP工作空间的所有目录
    static int ftp_list(SOCKET c_sock, char* path, char** data, int* data_len);
    //删除目录
    static int ftp_deletefolder(SOCKET sock, char* path);
    //删除文件
    static int ftp_deletefile(SOCKET sock, char* filename);
    //修改文件/目录&移动文件/目录
    static int ftp_renamefile(SOCKET sock, char* s, char* d);
    //从服务器复制文件到本地 RETR
    static int ftp_server2local(SOCKET c_sock, char* s, char* d, int* size);
    //从本地复制文件到服务器 STOR
    static int ftp_local2server(SOCKET c_sock, char* s, char* d, int* size);
    //获取响应码
    static int ftp_recv(SOCKET sock, char* re_buf, signed long* len);

    

};
}  // namespace ELFTP
