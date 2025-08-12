#include <iostream>
#include <limits> // 用于 numeric_limits
#include "WorkoutCalculator.h"
#include "running_data.h"
#include "cycling_data.h"

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

    int choice = 0;
    std::cout << "请选择要计算的运动类型:" << std::endl;
    std::cout << "1. 跑步" << std::endl;
    std::cout << "2. 骑行" << std::endl;
    std::cout << "请输入选项 (1 或 2): ";
    
    // --- 修改：使用更健壮的输入循环 ---
    while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
        std::cout << "输入无效，请输入 1 或 2: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    // 根据用户选择创建不同的计算器实例
    if (choice == 1) {
        WorkoutCalculator calculator("跑步", runningData);
        calculator.run();
    } else if (choice == 2) {
        WorkoutCalculator calculator("骑行", cyclingData);
        calculator.run();
    }

    return 0;
}