#ifndef APPLICATION_H
#define APPLICATION_H

#include "ui/OutputFormat.h"
#include "core/CalculationEngine.h"
#include "core/FoodConverter.h"
#include "core/FoodDataManager.h"
#include "core/OutputConfigManager.h" // 新增
#include "domain/WorkoutDomain.h"

#include <string>

class Application {
public:
    // 修改构造函数以接受两个配置文件路径
    Application(const std::string& foodDataPath, const std::string& outputConfigPath);

    // 运行一次性的计算流程 (可用于CLI或测试)
    void runOnce(const std::string& sportType, const WorkoutParameters& params);

private:
    OutputFormat ui;
    CalculationEngine engine;
    FoodConverter foodConverter;
    FoodDataManager foodDataManager_;
    OutputConfigManager outputConfigManager_; // 新增

    // 私有辅助函数，用于验证输入参数的合法性
    bool validateParameters(const WorkoutParameters& params);
};

#endif // APPLICATION_H