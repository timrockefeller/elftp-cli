#include <ArgHandle.h>
#include <FTPAPI.h>
#include <View.h>
#include <windows.h>
#include <exception>
//#include "stdafx.h"
using namespace std;
using namespace ELFTP;

#define VERSION "0.1"

int main(int argc, char* argv[]) {
    // SOCKET so =  FTPAPI::socket_connect("120.0.0.1", 27);
    View::getInstance()->Init();

    // bind commands
    ArgHandle::getInstance()
        ->BindCommand("help", 0, [&](vector<string> args) {
            cout << "ELFTP client commands:" << endl << endl;
            cout << " jump   <ip> <port> [un] [pw]  --- Connect to a server." << endl;
            cout << "   ls   [dir]                  --- List all files at cwd on server." << endl;
            cout << "  lls   [dir]                  --- List all files at cwd on client." << endl;
            cout << "   cd   <dir>                  --- Change current working directory on server." << endl;
            cout << "  lcd   <dir>                  --- Change current working directory on client." << endl;
            cout << "  pwd                          --- Show current working directory on server." << endl;
            cout << " lpwd                          --- Show current working directory on client." << endl;
            cout << "  get   <filename> [target]    --- Download a file from server." << endl;
            //cout << "  put   <filename> [target]    --- Upload a file to server." << endl;
            cout << "  bye                          --- Exit the client." << endl;
            cout << endl;
        })
        //和通用客户端统一
        ->BindCommand("bye", 0, [&](vector<string> args) {
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
            SOCKET s = ArgHandle::getInstance()->getSocket();
            if(s != 0) { //断开上一个连接
                FTPAPI::ftp_quit(s);
            }
            char ip [16];
            strcpy(ip,args[0].c_str());
            char un [30];
            strcpy(un,args[2].c_str());
            char pw [30];
            strcpy(pw,args[3].c_str());
            s = FTPAPI::ftp_connect(ip,atoi(args[1].c_str()),un,pw); //登录到FTP服务器
            ArgHandle::getInstance()->setSocket(s);
        }, {"", "anonymous"})
        ->BindCommand("cd", 1, [&](vector<string> args) {
            // TODO CWD相关
            // 要分析 . 和 .. 还有相对目录问题，可能要一个静态类存这些东西
            //current_dict = 

        })
        ->BindCommand("lcd", 1, [&](vector<string> args) {
            // TODO CWD相关
            // 要分析 . 和 .. 还有相对目录问题，可能要一个静态类存这些东西
            //char current_dict[256] = "/";
            //_wgetcwd(current_dict,sizeof(current_dict));
            char cDict[1024] = "";
            GetCurrentDirectory(1024,cDict);
            std::string a = "";
            try{
                a.assign(cDict);
                a.append("\\");
                a.append(args[0].c_str());
                cout<<"LOCAL: GOTO -> "<<a.c_str()<<endl;
                SetCurrentDirectory(a.c_str());
            }
            catch (exception& e){
                cout<<"==WRONG PATH=="<<endl;
                cout << "Exit with exception: " << e.what() << endl;
            }

        })
        ->BindCommand("ls", 1, [&](vector<string> args) {
            // TODO LIST相关
            // args[0] - relative directory = "."
            // 列出内容呗

            
            SOCKET s = ArgHandle::getInstance()->getSocket();
            char command[60] = "";
            strcat(command,"ls ");
            strcat(command,args[0].c_str());
            //FTPAPI::ftp_sendcmd_re(s,command,"In current Dictionary:\n",1024);
            // int* p;
            // *p = 1025;
            try{
                FTPAPI::ftp_sendcmd(s,command);
            }
            catch (exception& e){
                cout << "Exit with exception: " << e.what() << endl;
            }

        }, {"."})
        ->BindCommand("lls", 1, [&](vector<string> args) {
            
            char command[1024] = "";
            strcat(command,"ls ");
            strcat(command,args[0].c_str());
            //cout<<cDict<<endl;
            cout<<"LOCAL: Content of Current Dictionary"<<endl;
            system(command);

        }, {"."})
        ->BindCommand("pwd", 0, [&](vector<string> args) {
            // TODO PWD相关
            // 打印当前目录
            SOCKET s = ArgHandle::getInstance()->getSocket();
            if(s == 0){
                cout<<"You haven't connect to server yet."<<endl;
            }else{

            }
            // char command[60] = "";
            // strcat(command,"ls ");
            // strcat(command,args[0].c_str());
            FTPAPI::ftp_sendcmd(s,"pwd");
        })
        ->BindCommand("lpwd", 0, [&](vector<string> args) {
            // TODO PWD相关
            // 打印当前目录
            char cDict[1024] = "";
            GetCurrentDirectory(1024,cDict);
            cout<<cDict<<endl;
        })
        ->BindCommand("get", 2, [&](vector<string> args) {
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
    cout << "ELFTP " << VERSION << endl;
    
    if (argc <= 1) {  // FIXME 是否考虑全局模式？
        cout << " type \"help\" for more informations.\n" << endl;
        ArgHandle::getInstance()->ReadArgs(true);
    } else {
        ArgHandle::getInstance()->Parse(argc, argv);
    }
    return 0;
}
