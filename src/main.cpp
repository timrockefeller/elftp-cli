#include <ArgHandle.h>
#include <FTPAPI.h>
#include <View.h>

using namespace std;
using namespace ELFTP;
int main(int argc, char* argv[]) {
    // SOCKET so =  FTPAPI::socket_connect("120.0.0.1", 27);
    View::getInstance()->Init();

    // bind commands
    ArgHandle::getInstance()
        ->BindCommand("help", 0, [&](vector<string> args) {
            cout << "Yes We Made It!!!" << endl;
        })
        ->BindCommand("exit", 0, [&](vector<string> args) {
            cout << "Bye!" << endl;
            ArgHandle::getInstance()->Exit();
        })
        ->BindCommand("echo", 1, [&](vector<string> args) {
            // args[0] - message = "Hello World!" : 输出这一句话
            cout << args[0] << endl;
        }, {"Hello World!"})
        ->BindCommand("jump", 4, [&](vector<string> args) {
            // 连接到一个FTP上
            // args[0] - IP
            // args[1] - Port
            // args[2] - Username = "anonymous" : 默认匿名登录
            // args[3] - Password = ""

            //TODO 断开上一个连接
            char ip [16];
            strcpy(ip,args[0].c_str());
            char un [30];
            strcpy(un,args[2].c_str());
            char pw [30];
            strcpy(pw,args[3].c_str());
            SOCKET s = FTPAPI::ftp_connect(ip,atoi(args[1].c_str()),un,pw); //登录到FTP服务器
            ArgHandle::getInstance()->setSocket(s);
        }, {"", "anonymous"})
        ->BindCommand("cd", 1, [&](vector<string> args) {
            // TODO CWD相关
            // 要分析 . 和 .. 还有相对目录问题，可能要一个静态类存这些东西

        })
        ->BindCommand("ls", 0, [&](vector<string> args) {
            // TODO LIST相关
            // 列出内容呗

        })
        ->BindCommand("retr", 2, [&](vector<string> args) {
            // TODO RETR文件下载
            // args[0] - source
            // args[1] - target = "./" : 以"/"结尾意味着直接放入文件夹，其他情况就改名如何
            // 也要处理一下目录问题
            int size;
            char Sou[50];
            strcpy(Sou, args[0].c_str());
            char Tar[50];
            strcpy(Tar, args[1].c_str());
            SOCKET s = ArgHandle::getInstance()->getSocket();
            FTPAPI::ftp_server2local(s, Sou, Tar, &size);
        }, {"./"});

    if (argc <= 1) {  // FIXME 是否考虑全局模式？
        ArgHandle::getInstance()->ReadArgs(true);
    } else {
        ArgHandle::getInstance()->Parse(argc, argv);
    }
    return 0;
}
