#include <ArgHandle.h>
#include <FTPAPI.h>
#include <View.h>
using namespace ELFTP;
using namespace std;

void ArgHandle::setSocket(SOCKET s){
    this->s = s;
}

SOCKET ArgHandle::getSocket(){
    return this->s;
}

void ArgHandle::Parse(const string cmd, const vector<string> argv) {
    map<string, Command*>::iterator iter = commands.find(cmd);
    if (iter != commands.end())
        if (iter->second->Invoke(argv))
            iter->second->Run();
        else
            View::getInstance()->PrintErr("Invalid number of argument!\n");
    else
        View::getInstance()->PrintErr("Unknow Command\n");
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
    /* 处理双引号
        有没有闲的，来做个算法题：
        给一个string，返回一个vector<string>，要求将string按空格分隔，有双引号的忽略内部空格。
        同时要处理首尾和内部多余空格
        输入:`  Ass    "We Can"  `
        输出:["Ass","We Can"]
    */
    int _spCur = 0;  // 空格起始位
    while (_spCur < line.length() && line.at(_spCur) == ' ')
        _spCur++;
    if (_spCur == line.length())
        return;
    string cmd;
    vector<string> arglist = vector<string>();
    int _npCur = _spCur;  // 非空格起始位
    // fetch cmd
    while (_spCur < line.length() && line.at(_spCur) != ' ')
        _spCur++;
    cmd = line.substr(_npCur, _spCur - _npCur);
    // fetch arglist
    while (_spCur < line.length()) {
        while (_spCur < line.length() && line.at(_spCur) == ' ')
            _spCur++;
        if (_spCur == line.length())
            break;
        _npCur = _spCur;
        if (line.at(_spCur) == '"') {
            _npCur++;
            _spCur++;
            while (_spCur < line.length() && line.at(_spCur) != '"')
                _spCur++;
            arglist.push_back(line.substr(_npCur, _spCur - _npCur));
            _spCur++;
        } else {
            while (_spCur < line.length() && line.at(_spCur) != ' ')
                _spCur++;
            arglist.push_back(line.substr(_npCur, _spCur - _npCur));
        }
    }
    this->Parse(cmd, arglist);
}

void ArgHandle::ReadArgs(bool isLoop) {
    // 内置命令行读取部分，要先调用View的组件。
    this->isEOF = !isLoop;
    do {
        string cmdline;
        View::getInstance()->Input(cmdline);
        Parse(cmdline);
    } while (!this->isEOF);
}

ArgHandle* ArgHandle::BindCommand(string cmd,
                                  int argc,
                                  std::function<void(vector<string>)>&& op_,
                                  vector<string> autofill) {
    // 可以在这里加desc
    Command* c = new Command(argc, op_);
    if(autofill.size()!=0){
        c->LoadAutofill(autofill);
    }
    commands.insert(pair<string, Command*>(cmd, c));
    return getInstance();  // in default
}

void ArgHandle::Exit() {
    this->isEOF = true;
}
