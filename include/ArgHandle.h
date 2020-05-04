#include <Singleton.h>

#include <string>
namespace ELFTP {
class ArgHandle : public Singleton<ArgHandle> {
   protected:
    bool isEOF = false;

   public:
    void Parse(char** args);

    void ReadArgs(bool isLoop = false);
};
}  // namespace ELFTP
