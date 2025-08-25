#include "ui/ConsoleUI.h"
#include <print>      // Replaced <iostream> and <iomanip>
#include <cstdio>     // For stderr
#include <limits>
#include <vector>
#include <string>

// Note: std::cin is still used for input and requires its own headers/setup.
// This refactor only focuses on replacing std::cout/std::cerr.
#include <iostream> 

namespace { // Using an anonymous namespace to hide helper functions
    template<typename T>
    void getNumericInput(T& value, const std::string& prompt) {
        std::print("{}", prompt);
        while (!(std::cin >> value)) {
            std::print("输入无效，请输入一个合法的数字: ");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Shows version information
void ConsoleUI::displayVersionInfo(const std::string& version, const std::string& lastUpdated) {
    std::println("\n========================================");
    std::println("[ 版本信息 ]");
    std::println("  - 当前版本: {}", version);
    std::println("  - 最后更新: {}", lastUpdated);
    std::println("========================================\n");
}

// Shows a goodbye message
void ConsoleUI::displayGoodbyeMessage() {
    std::println("\n感谢使用，再见！\n");
}

// Gets the user's sport choice, now includes options for version and exit
int ConsoleUI::getSportChoice() {
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

    // Loop until the user enters a valid number between 0 and 3
    while (!(std::cin >> choice) || choice < 0 || choice > 3) {
        std::print("输入无效，请输入 0, 1, 2, 或 3: ");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return choice;
}

WorkoutParameters ConsoleUI::getWorkoutParameters(const std::string& activityName) {
    WorkoutParameters params;
    std::println(""); // Add a newline to separate the menu from input
    getNumericInput(params.timeHr, std::format("请输入您的{}时间（小时部分）: ", activityName));
    getNumericInput(params.timeMin, std::format("请输入您的{}时间（分钟部分）: ", activityName));
    getNumericInput(params.timeSec, std::format("请输入您的{}时间（秒的部分）: ", activityName));
    getNumericInput(params.distanceKm, std::format("请输入您的{}距离（公里）: ", activityName));
    getNumericInput(params.weightKg, "请输入您的体重（公斤）: ");
    return params;
}

void ConsoleUI::displayError(const std::string& message) {
    // Direct output to the standard error stream (stderr)
    std::println(stderr, "\n错误：{}", message);
}

void ConsoleUI::displaySpeedError(double userSpeed, double minSpeed, double maxSpeed) {
    // {:.1f} formats the numbers to one decimal place
    std::println("\n----------------------------------------");
    std::println("您的平均速度是: {:.1f} 公里/小时 (km/h)", userSpeed);
    std::println("\n错误：您的速度不在有效计算范围内。");
    std::println("本程序可计算的速度范围为 {:.1f} 到 {:.1f} 公里/小时。", minSpeed, maxSpeed);
    std::println("----------------------------------------");
}

void ConsoleUI::displayResults(const WorkoutResults& results,
                               const std::string& activityName,
                               const std::map<std::string, std::vector<FoodEquivalent>>& foodEquivalents) {
    const int labelWidth = 28;

    std::println("\n========================================");
    std::println("           运动热量计算报告           ");
    std::println("========================================");

    std::println("\n[ 运动表现 ]");
    // {:<{}} is used for left-alignment with a dynamic width
    // {:.1f} is used for floating-point precision
    std::println("{:<{}} {:.1f}", "  - 平均速度 (km/h): ", labelWidth, results.userSpeedKmh);
    std::println("{:<{}} {:.1f}", "  - 平均速度 (m/s): ", labelWidth, results.speedMps);
    // 配速保留两位小数
    std::println("  - 平均配速: {}分{:.2f}秒 / 公里", results.paceMinutes, results.paceSeconds);

    std::println("\n[ 热量消耗 ]");
    std::println("{:<{}} {:.1f}", "  - 平均代谢当量 (METs): ", labelWidth, results.averageMets);
    std::println("{:<{}} {:.1f}", "  - 总消耗 (千卡/大卡): ", labelWidth, results.totalKcal);
    std::println("{:<{}} {:.1f}", "  - 总消耗 (千焦): ", labelWidth, results.totalKj);

    std::println("\n[ 食物等效 ]");
    std::println("  本次消耗的热量，大约相当于：");

    for (const auto& pair : foodEquivalents) {
        const std::string& categoryName = pair.first;
        const std::vector<FoodEquivalent>& foods = pair.second;

        std::println("\n  --- {} ---", categoryName);
        for (const auto& food : foods) {
            std::println("    - {:.1f} {} {} ({} kcal/100{})",
                         food.amount, food.unit, food.name,
                         static_cast<int>(food.kcalPer100),
                         (food.unit == "克" ? "g" : "ml"));
        }
    }

    std::println("\n[ 等效活动 ]");
    std::println("  本次{}从热量消耗来看，", activityName);
    std::println("  相当于静坐了：{} 小时 {} 分钟", results.equivalentHours, results.equivalentMinutes);

    std::println("========================================\n");
}