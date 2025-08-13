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
    FoodConverter foodConverter;

    // 私有辅助函数，用于验证输入参数的合法性
    bool validateParameters(const WorkoutParameters& params);

    // 新增：私有辅助函数，用于处理单次运动计算流程
    void processWorkout(int sportChoice); 
};

#endif // APPLICATION_H