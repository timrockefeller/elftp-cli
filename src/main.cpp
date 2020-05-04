#include <ArgHandle.h>
#include <FTPAPI.h>
#include <View.h>
using namespace ELFTP;
int main(int argc, char* argv[]) {
    // SOCKET so =  FTPAPI::socket_connect("120.0.0.1", 27);

    if (argc <= 1) {  // TODO 全局模式
        ArgHandle::getInstance()->ReadArgs(true);
    } else {
        ArgHandle::getInstance()->Parse(argv + 1);
    }

    View::getInstance()->Init();
    View::getInstance()->PrintLayout();
    return 0;
}
