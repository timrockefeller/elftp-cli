#include <ArgHandle.h>
#include <FTPAPI.h>

using namespace ELFTP;
using namespace std;

void ArgHandle::Parse(char** args) {

}

void ArgHandle::ReadArgs(bool isLoop) {
    do {
        // 读取下一行arg
        this->isEOF = true;
    } while (isLoop && !this->isEOF);
}
