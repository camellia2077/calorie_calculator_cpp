#ifndef APPLICATION_H
#define APPLICATION_H

#include "ConsoleUI.h"
#include "core/CalculationEngine.h"

class Application {
public:
    // 运行主程序流程
    void run();

private:
    ConsoleUI ui;
    CalculationEngine engine;

    // 私有辅助函数，用于验证输入参数的合法性
    bool validateParameters(const WorkoutParameters& params);
};

#endif // APPLICATION_H