#ifndef APPLICATION_H
#define APPLICATION_H

#include "ui/ConsoleUI.h"
#include "core/CalculationEngine.h"
#include "core/FoodConverter.h"
#include "domain/WorkoutDomain.h"
#include <string>

class Application {
public:
    // 运行交互式主程序流程
    void run();

    // 新增：运行一次性的计算流程 (可用于CLI或测试)
    void runOnce(const std::string& sportType, const WorkoutParameters& params);

private:
    ConsoleUI ui;
    CalculationEngine engine;
    FoodConverter foodConverter;

    // 私有辅助函数，用于验证输入参数的合法性
    bool validateParameters(const WorkoutParameters& params);

    // 这个函数现在会调用 runOnce
    void processWorkout(int sportChoice);
};

#endif // APPLICATION_H