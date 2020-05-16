### Introduction

An FTP client tool.

### ストーリー

昔々、小さいエルフはどこへ行くか分からない。

今、僕たちは彼女がテレポートなったことを知っています．．．


### Dependences

Dev Tools:
- GCC (C++11) or Visual Studio 2015+ (msvc included)
- CMake 3.0.0+

VsCode Extentions:
- **CMake Tools** 1.3.1
- CMake 0.0.17
- C/C++ 0.28.0

### Requirements

- 以命令行或图形界面形式运行程序
- 要求在客户端中至少实现CWD、LIST、RETR功能。
- 输出与服务器交互过程中的命令与应答信息，下载指定FTP网站的指定文件。

### Deploy

```powershell
mkdir build
cd build
cmake ..
```

### License

Copyright 2020 by Kitekii, Cyame, DariusYoung, xuchenyangnb. All Rights Reserved.
