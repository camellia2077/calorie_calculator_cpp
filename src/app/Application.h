#ifndef APPLICATION_H
#define APPLICATION_H

#include "ui/ConsoleUI.h"
#include "core/CalculationEngine.h"
#include "core/FoodConverter.h"
#include "domain/WorkoutDomain.h"

class Application {
public:
    // 运行主程序流程
    void run();

private:
    ConsoleUI ui;
    CalculationEngine engine;
    FoodConverter foodConverter; // It's better to make this a member variable

    // 私有辅助函数，用于验证输入参数的合法性
    bool validateParameters(const WorkoutParameters& params);
};

#endif // APPLICATION_H