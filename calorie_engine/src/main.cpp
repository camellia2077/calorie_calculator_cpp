#include "app/Application.h"
#include "common/version.h"
#include "ui/OutputFormat.h" // 包含UI头文件以使用其显示功能
#include <iostream>
#include <limits>
#include <string>
#include <print>

// 为 Windows 平台引入头文件
#ifdef _WIN32
#include <windows.h>
#endif

// --- 将交互逻辑移至 main.cpp ---

namespace { // 使用匿名命名空间来隐藏这些辅助函数

    // 从 OutputFormat.cpp 移动过来的辅助函数
    template<typename T>
    void getNumericInput(T& value, const std::string& prompt) {
        std::print("{}", prompt);
        while (!(std::cin >> value)) {
            std::print("输入无效，请输入一个合法的数字: ");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // 从 OutputFormat.cpp 移动过来的逻辑
    int getSportChoice() {
        int choice = -1;
        std::println("----------------------------------------");
        std::println("请选择操作:");
        std::println("  1. 跑步");
        std::println("  2. 骑行");
        std::println("--------------------");
        std::println("  0. 查看版本信息");
        std::println("  3. 退出程序");
        std::println("----------------------------------------");
        std::print("请输入选项 (0-3): ");

        while (!(std::cin >> choice) || choice < 0 || choice > 3) {
            std::print("输入无效，请输入 0, 1, 2, 或 3: ");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return choice;
    }

    // 从 OutputFormat.cpp 移动过来的逻辑
    WorkoutParameters getWorkoutParameters(const std::string& activityName) {
        WorkoutParameters params;
        std::println(""); // 添加换行符
        getNumericInput(params.timeHr, std::format("请输入您的{}时间（小时部分）: ", activityName));
        getNumericInput(params.timeMin, std::format("请输入您的{}时间（分钟部分）: ", activityName));
        getNumericInput(params.timeSec, std::format("请输入您的{}时间（秒的部分）: ", activityName));
        getNumericInput(params.distanceKm, std::format("请输入您的{}距离（公里）: ", activityName));
        getNumericInput(params.weightKg, "请输入您的体重（公斤）: ");
        return params;
    }
}

int main() {
    // 环境设置
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    // 使用新的构造函数实例化 Application 对象，并传入食物数据路径
    Application app("food_data.json");
    OutputFormat ui; // 创建一个UI实例用于显示

    // 从 Application::run() 移动过来的主循环
    while (true) {
        int choice = getSportChoice();
        std::string sportType;
        std::string activityName;

        switch (choice) {
            case 0: {
                ui.displayVersionInfo(AppVersion::getVersionString(), AppVersion::LAST_UPDATED);
                break;
            }
            case 1: { // 跑步
                sportType = "run";
                activityName = "跑步";
                WorkoutParameters params = getWorkoutParameters(activityName);
                app.runOnce(sportType, params); // 调用核心计算逻辑
                break;
            }
            case 2: { // 骑行
                sportType = "bike";
                activityName = "骑行";
                WorkoutParameters params = getWorkoutParameters(activityName);
                app.runOnce(sportType, params); // 调用核心计算逻辑
                break;
            }
            case 3: { // 退出
                ui.displayGoodbyeMessage();
                return 0;
            }
            default: {
                // 不太可能发生，因为 getSportChoice 已经处理了无效输入
                ui.displayError("无效的选项，请重新输入。");
                break;
            }
        }
    }

    return 0;
}