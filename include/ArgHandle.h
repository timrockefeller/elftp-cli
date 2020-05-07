#include <Singleton.h>
#include <FTPAPI.h>

#include <functional>
#include <map>
#include <string>
#include <vector>
namespace ELFTP {

class Command {
   private:
    std::string cmd;
    std::vector<std::string> argv;
    std::vector<std::string> autofill;
    int argc;
    std::function<void(std::vector<std::string>)> op;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

   public:
    Command() = default;
    Command(int argnum, const std::function<void(std::vector<std::string>)>& op_);   
    void LoadAutofill(std::vector<std::string> autofill); 
    // 载入参数
    bool Invoke(std::vector<std::string>);
    // 运行
    // note: 执行前必须使用Invoke
    bool Run();
};

class ArgHandle : public Singleton<ArgHandle> {
   protected:
    bool isEOF = false;
    std::map<std::string, Command*> commands;
    SOCKET s;
   public:
    void setSocket(SOCKET s);
    SOCKET getSocket();
    // final参数部署
    void Parse(const std::string cmd, const std::vector<std::string> argv);
    // 来自run arg的参数，旨在运行一次就结束
    void Parse(int argc, char** argv);
    // 输入一行内容，根据首个单词确定
    void Parse(std::string line);
    // 内置命令行模式
    void ReadArgs(bool isLoop = false);
    /**
     * 绑定命令
     * - cmd: 命令
     * - argc: 要求的参数个数
     * - op_: 执行函数
     * - autofill: 缺少参数时填充，如op([A,B,C,D])若要将C、D两项加以自动填充，则需要传入倒序{D,C}
     * */
    ArgHandle* BindCommand(
        std::string cmd,
        int argc,
        std::function<void(std::vector<std::string>)>&& op_,
        std::vector<std::string> autofill = {});
    // 顾名思义
    void Exit();
};
}  // namespace ELFTP
