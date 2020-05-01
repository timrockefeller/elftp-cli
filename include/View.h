#include <windows.h>

#include <iostream>
using namespace std;

namespace ELFTP {
template <typename T, typename = std::enable_if<std::is_class<T>::value>>
class Singleton {
   public:
    static T* getInstance() noexcept {
        static T* instance = new T;
        return instance;
    }

   protected:
    Singleton() = default;
    virtual ~Singleton() = default;

   private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

class View : public Singleton<View> {
   protected:
    HANDLE stdHandle;

   public:
    void Init();
    // Essential Functions
    void MoveCursorTo(int x, int y);

    void ColorPrintf(char* str, WORD cl = FOREGROUND_BLUE|FOREGROUND_INTENSITY);
    // 输出窗体
    void PrintLayout();
};
}  // namespace ELFTP
