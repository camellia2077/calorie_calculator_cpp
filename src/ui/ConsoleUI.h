#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <string>
#include <vector>
#include <map>
#include "domain/WorkoutDomain.h"
#include "domain/FoodEquivalent.h" 

class ConsoleUI {
public:
    // --- New Functions ---
    // 显示版本信息
    void displayVersionInfo(const std::string& version, const std::string& lastUpdated);

    // 显示退出信息
    void displayGoodbyeMessage();

    // --- Existing Functions ---
    // 获取用户选择的运动类型
    int getSportChoice();

    // 获取用户的运动数据
    WorkoutParameters getWorkoutParameters(const std::string& activityName);

    // 显示最终的计算报告
    void displayResults(const WorkoutResults& results,
                        const std::string& activityName,
                        const std::map<std::string, std::vector<FoodEquivalent>>& foodEquivalents);

    // 显示错误信息
    void displayError(const std::string& message);

    // 显示速度超出范围的特定错误
    void displaySpeedError(double userSpeed, double minSpeed, double maxSpeed);
};

#endif // CONSOLE_UI_H