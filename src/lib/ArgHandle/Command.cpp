#include <ArgHandle.h>

using namespace ELFTP;
using namespace std;

Command::Command(int argnum, const std::function<void(vector<string>)>& op_) {
    this->argc = argnum;
    this->argv = vector<string>();
    this->op = op_;
}

bool Command::Invoke(vector<string> arglist) {
    if (0 == this->argc ||
        arglist.size() == this->argc) {  // 检测参数个数是否正确，考虑到无参函数可以无条件运行
        this->argv = arglist;
        return true;
    }
    return false;
}

bool Command::Run() {
    try {
        this->op(argv);
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}
