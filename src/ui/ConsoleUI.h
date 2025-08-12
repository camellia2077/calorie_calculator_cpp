#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <string>
#include "domain/WorkoutDomain.h"

// 用户界面的头文件，负责声明所有与输入输出相关的功能。

class ConsoleUI {
public:
    // 获取用户选择的运动类型
    int getSportChoice();

    // 获取用户的运动数据
    WorkoutParameters getWorkoutParameters(const std::string& activityName);

    // 显示最终的计算报告
    void displayResults(const WorkoutResults& results, const std::string& activityName);

    // 显示错误信息
    void displayError(const std::string& message);
    
    // 显示速度超出范围的特定错误
    void displaySpeedError(double userSpeed, double minSpeed, double maxSpeed);
};

#endif // CONSOLE_UI_H