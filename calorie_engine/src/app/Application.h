#ifndef APPLICATION_H
#define APPLICATION_H

#include "ui/OutputFormat.h"
#include "core/CalculationEngine.h"
#include "core/FoodConverter.h"
#include "core/FoodDataManager.h"
#include "domain/WorkoutDomain.h"

#include <string>

class Application {
public:
    // 新增的构造函数声明
    Application(const std::string& foodDataPath);

    // 运行一次性的计算流程 (可用于CLI或测试)
    void runOnce(const std::string& sportType, const WorkoutParameters& params);

private:
    OutputFormat ui;
    CalculationEngine engine;
    FoodConverter foodConverter;
    FoodDataManager foodDataManager_;

    // 私有辅助函数，用于验证输入参数的合法性
    bool validateParameters(const WorkoutParameters& params);
};

#endif // APPLICATION_H