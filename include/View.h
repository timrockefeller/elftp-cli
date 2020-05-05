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

    /* Essential Functions */
    // 移动指针
    void MoveCursorTo(int x, int y);
    // 彩色输出
    void ColorPrintf(char* str,
                     WORD cl = FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    /* Draw Functions */
    // 输出一个矩阵边框
    void PrintBorder(char ch, int x, int y, int w, int h);
    // 清空一个区域
    void Clear(int x, int y, int w, int h);
    // 清空全部区域
    void ClearScreen();
    // 带区域限制输出文本
    void PrintTextarea(char*,
                       int x,
                       int y,
                       int w = -1,
                       int h = -1,
                       bool wordwarp = true);

    /* Application Functions */
    // 输出窗体
    void PrintLayout();
    // 输出信息
    void PrintLog();
    // 输出错误信息
    void PrintErr();
};
}  // namespace ELFTP
