#include "Application.h"

// 为 Windows 平台引入头文件
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // 环境设置
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    Application app;
    app.run();

    return 0;
}