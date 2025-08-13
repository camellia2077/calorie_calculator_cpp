#include "ui/ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

namespace { // 使用匿名命名空间来隐藏辅助函数
    template<typename T>
    void getNumericInput(T& value, const std::string& prompt) {
        std::cout << prompt;
        while (!(std::cin >> value)) {
            std::cout << "输入无效，请输入一个合法的数字: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// *** NEW IMPLEMENTATION ***
// 显示版本信息
void ConsoleUI::displayVersionInfo(const std::string& version, const std::string& lastUpdated) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "[ 版本信息 ]" << std::endl;
    std::cout << "  - 当前版本: " << version << std::endl;
    std::cout << "  - 最后更新: " << lastUpdated << std::endl;
    std::cout << "========================================\n" << std::endl;
}

// *** NEW IMPLEMENTATION ***
// 显示告别信息
void ConsoleUI::displayGoodbyeMessage() {
    std::cout << "\n感谢使用，再见！\n" << std::endl;
}

// *** MODIFIED FUNCTION ***
// getSportChoice 现在包含版本和退出选项
int ConsoleUI::getSportChoice() {
    int choice = -1;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "请选择操作:" << std::endl;
    std::cout << "  1. 跑步" << std::endl;
    std::cout << "  2. 骑行" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "  0. 查看版本信息" << std::endl;
    std::cout << "  3. 退出程序" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "请输入选项 (0-3): ";

    // 循环直到用户输入一个在 0 和 3 之间的有效数字
    while (!(std::cin >> choice) || choice < 0 || choice > 3) {
        std::cout << "输入无效，请输入 0, 1, 2, 或 3: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return choice;
}

WorkoutParameters ConsoleUI::getWorkoutParameters(const std::string& activityName) {
    WorkoutParameters params;
    std::cout << std::endl; // 添加换行以分隔菜单和输入
    getNumericInput(params.timeHr, "请输入您的" + activityName + "时间（小时部分）: ");
    getNumericInput(params.timeMin, "请输入您的" + activityName + "时间（分钟部分）: ");
    getNumericInput(params.timeSec, "请输入您的" + activityName + "时间（秒的部分）: ");
    getNumericInput(params.distanceKm, "请输入您的" + activityName + "距离（公里）: ");
    getNumericInput(params.weightKg, "请输入您的体重（公斤）: ");
    return params;
}

void ConsoleUI::displayError(const std::string& message) {
    std::cerr << "\n错误：" << message << std::endl;
}

void ConsoleUI::displaySpeedError(double userSpeed, double minSpeed, double maxSpeed) {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "您的平均速度是: " << userSpeed << " 公里/小时 (km/h)" << std::endl;
    std::cout << "\n错误：您的速度不在有效计算范围内。" << std::endl;
    std::cout << "本程序可计算的速度范围为 " << minSpeed << " 到 " << maxSpeed << " 公里/小时。" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void ConsoleUI::displayResults(const WorkoutResults& results,
                               const std::string& activityName,
                               const std::map<std::string, std::vector<FoodEquivalent>>& foodEquivalents) {
    const int labelWidth = 28;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "\n========================================" << std::endl;
    std::cout << "           运动热量计算报告           " << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "\n[ 运动表现 ]" << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 平均速度 (km/h): " << results.userSpeedKmh << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 平均速度 (m/s): " << results.speedMps << std::endl;
    std::cout << std::left << std::setw(0) << "  - 平均配速: " << results.paceMinutes << "分" << results.paceSeconds << "秒 / 公里" << std::endl;

    std::cout << "\n[ 热量消耗 ]" << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 平均代谢当量 (METs): " << results.averageMets << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 总消耗 (千卡/大卡): " << results.totalKcal << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 总消耗 (千焦): " << results.totalKj << std::endl;

    std::cout << "\n[ 食物等效 ]" << std::endl;
    std::cout << "  本次消耗的热量，大约相当于：" << std::endl;

    for (const auto& pair : foodEquivalents) {
        const std::string& categoryName = pair.first;
        const std::vector<FoodEquivalent>& foods = pair.second;

        std::cout << "\n  --- " << categoryName << " ---" << std::endl;
        for (const auto& food : foods) {
            std::cout << "    - " << std::fixed << std::setprecision(1) << food.amount << " " << food.unit << " " << food.name
                      << " (" << static_cast<int>(food.kcalPer100) << " kcal/100" << (food.unit == "克" ? "g" : "ml") << ")" << std::endl;
        }
    }

    std::cout << "\n[ 等效活动 ]" << std::endl;
    std::cout << "  本次" << activityName << "从热量消耗来看，" << std::endl;
    std::cout << "  相当于静坐了：" << results.equivalentHours << " 小时 " << results.equivalentMinutes << " 分钟" << std::endl;

    std::cout << "========================================\n" << std::endl;
}