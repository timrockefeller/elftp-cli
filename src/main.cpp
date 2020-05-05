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
            cout << args[0] << endl;
        });

    if (argc <= 1) {  // FIXME 是否考虑全局模式？
        ArgHandle::getInstance()->ReadArgs(true);
    } else {
        ArgHandle::getInstance()->Parse(argc, argv);
    }
    return 0;
}
