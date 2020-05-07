#include <ArgHandle.h>

using namespace ELFTP;
using namespace std;

Command::Command(int argnum, const std::function<void(vector<string>)>& op_) {
    this->argc = argnum;
    this->argv = vector<string>();
    this->op = op_;
    this->autofill = {};
}

void Command::LoadAutofill(vector<string> autofill){
    if(autofill.size()<=argc)
        this->autofill = autofill;
}

bool Command::Invoke(vector<string> arglist) {
    if (0 == this->argc ||
        arglist.size() <= this->argc &&
        arglist.size() >= this->argc - this->autofill.size()) {  // 检测参数个数是否正确，考虑到无参函数可以无条件运行
        this->argv = arglist;
        for(int i = arglist.size();i<this->argc;i++){
            // 0 1 2 3 4 5
            // A B C(D E F)
            // F E D
            // A B C D
            //         E F
            this->argv.push_back(this->autofill[this->argc - i - 1]);
        }
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
