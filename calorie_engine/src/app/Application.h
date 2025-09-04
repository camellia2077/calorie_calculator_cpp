#ifndef APPLICATION_H
#define APPLICATION_H

#include "ui/OutputFormat.h"
#include "core/CalculationEngine.h"
#include "core/FoodConverter.h"
#include "core/FoodDataManager.h"
#include "core/OutputConfigManager.h"
#include "domain/WorkoutDomain.h"
// 新增: 包含外观头文件
#include "db_inserter/DbInserterFacade.h"

#include <string>

class Application {
public:
    // 构造函数保持不变
    Application(const std::string& foodDataPath, const std::string& outputConfigPath);

    // 运行一次性的计算流程
    void runOnce(const std::string& sportType, const WorkoutParameters& params);

    // 新增: 运行一次性的导入流程
    void importActivityFromJson(const std::string& jsonFilePath, const std::string& dbPath);

private:
    OutputFormat ui;
    CalculationEngine engine;
    FoodConverter foodConverter;
    FoodDataManager foodDataManager_;
    OutputConfigManager outputConfigManager_;

    // 私有辅助函数保持不变
    bool validateParameters(const WorkoutParameters& params);
};

#endif // APPLICATION_H