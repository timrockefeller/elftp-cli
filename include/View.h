#include <Singleton.h>
#include <windows.h>

#include <iostream>
using namespace std;

namespace ELFTP {

class View : public Singleton<View> {
   protected:
    HANDLE stdHandle;

   public:
    void Init();
    // Essential Functions
    void MoveCursorTo(int x, int y);

    void ColorPrintf(char* str,
                     WORD cl = FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 输出一个矩阵边框
    void PrintBorder(char ch, int x, int y, int w, int h);

    // 输出窗体
    void PrintLayout();
};
}  // namespace ELFTP
