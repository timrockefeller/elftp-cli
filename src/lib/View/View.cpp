#include <View.h>

using namespace ELFTP;

void View::Init() {
    stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void View::MoveCursorTo(int x, int y) {
    COORD cp = {short(x), short(y)};
    SetConsoleCursorPosition(stdHandle, cp);
}

void View::ColorPrintf(char* str,
                       WORD cl = FOREGROUND_BLUE | FOREGROUND_INTENSITY) {
    SetConsoleTextAttribute(stdHandle, cl);
    cout << str;
    SetConsoleTextAttribute(stdHandle,
                            FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
}

void View::PrintLayout() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(stdHandle, &csbi);
    int x = csbi.dwSize.X;
    MoveCursorTo(0, 0);
    while (x-- > 0)
        ColorPrintf("#", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    x = csbi.dwSize.X;
    MoveCursorTo(0, csbi.dwSize.Y - 4);
    while (x-- > 0) {
        ColorPrintf("#", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
}
