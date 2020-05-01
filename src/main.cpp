#include <FTPAPI.h>
#include <View.h>
using namespace ELFTP;
int main() {
    // SOCKET so =  FTPAPI::socket_connect("120.0.0.1", 27);
    View::getInstance()->Init();
    View::getInstance()->PrintLayout();
    return 0;
}
