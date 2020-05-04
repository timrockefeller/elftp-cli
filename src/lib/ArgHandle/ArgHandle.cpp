#include <ArgHandle.h>
#include <FTPAPI.h>
#include <View.h>
using namespace ELFTP;
using namespace std;

void ArgHandle::Parse(const string cmd, const vector<string> argv) {
    map<string, Command*>::iterator iter = commands.find(cmd);
    if (iter != commands.end())
        if (iter->second->Invoke(argv))
            iter->second->Run();
    // TODO else View:: ErrorMsg(...)
}

void ArgHandle::Parse(int argc, char** argv) {
    string cmd = argv[1];
    vector<string> argvs = vector<string>();
    for (int i = 2; i < argc; i++) {
        argvs.push_back(argv[i]);
    }
    Parse(cmd, argvs);
}

void ArgHandle::Parse(string line) {
    char** arglist = new char*[5];
    int argcount;
    // := line.split(" ");
    // TODO 把line以空格为单位分割成字符串
    // line.substr()

    this->Parse(argcount, arglist);
}

void ArgHandle::ReadArgs(bool isLoop) {
    // TODO 内置命令行读取部分，要先调用View的组件。
    do {
        // View::CommandLine() FIXME
        // 读取下一行arg
        this->isEOF = true;
    } while (isLoop && !this->isEOF);
}

ArgHandle* ArgHandle::BindCommand(string cmd,
                            int argc,
                            std::function<void(vector<string>)>&& op_) {
    // 可以在这里加desc
    commands.insert(pair<string, Command*>(cmd,new Command(argc,op_)));
    return getInstance();// in default
}


void ArgHandle::Exit(){
    this->isEOF = true;
}
