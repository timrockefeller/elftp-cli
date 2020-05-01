#include <FTPAPI.h>
using namespace ELFTP;
int main() {
    SOCKET so =  FTPAPI::socket_connect("120.0.0.1", 27);
    return 0;
}
