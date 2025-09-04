#include "app/Application.h"
#include "common/version.h"
#include "data/running_data.h"
#include "data/cycling_data.h"
#include <string>
#include <vector>
#include <iostream> // 用于成功消息

// 构造函数保持不变
Application::Application(const std::string& foodDataPath, const std::string& outputConfigPath)
    : foodDataManager_(foodDataPath), outputConfigManager_(outputConfigPath) {}

// 新增的导入方法实现
void Application::importActivityFromJson(const std::string& jsonFilePath, const std::string& dbPath) {
    try {
        DbInserterFacade::importFromJson(jsonFilePath, dbPath);
        // 使用 std::cout 打印成功消息，因为 ui 对象没有通用的成功消息显示方法
        std::cout << "数据已成功导入数据库。" << std::endl;
    } catch (const std::exception& e) {
        // 使用 ui 对象来显示错误
        ui.displayError("导入失败: " + std::string(e.what()));
    }
}


// runOnce 方法保持不变
void Application::runOnce(const std::string& sportType, const WorkoutParameters& params) {
    // ... (代码前部分保持不变)

    // 1. 根据运动类型选择数据表和名称
    std::string activityName;
    const std::vector<DataPoint>* dataTable = nullptr;

    if (sportType == "run") {
        activityName = "跑步";
        dataTable = &runningData;
    } else if (sportType == "bike") {
        activityName = "骑行";
        dataTable = &cyclingData;
    } else {
        ui.displayError("内部错误：无效的运动类型。");
        return;
    }
    
    // 2. 验证输入参数的合法性
    if (!validateParameters(params)) {
        ui.displayError("输入值不合法。时间不能为负，分钟和秒数需小于60，距离和体重必须为正数。");
        return;
    }

    // 3. 将参数传递给计算引擎
    WorkoutResults results = engine.calculate(params, *dataTable);

    // 4. 验证计算出的速度是否在可计算范围内
    if (!dataTable->empty() && (results.userSpeedKmh < dataTable->front().speedKph || results.userSpeedKmh > dataTable->back().speedKph)) {
        ui.displaySpeedError(results.userSpeedKmh, dataTable->front().speedKph, dataTable->back().speedKph);
        return;
    }

    // 5. 调用食物转换器
    auto foodEquivalents = foodConverter.calculate(
        results.totalKcal,
        foodDataManager_.getFoodData()
    );

    // 6. 获取输出配置
    const auto& outputConfig = outputConfigManager_.getOutputConfig();

    // 7. 将所有结果和配置传递给UI进行显示
    ui.displayResults(results, activityName, foodEquivalents, outputConfig);
}

// validateParameters 方法保持不变
bool Application::validateParameters(const WorkoutParameters& params) {
    if (params.timeHr < 0 || params.timeMin < 0 || params.timeMin >= 60 ||
        params.timeSec < 0 || params.timeSec >= 60 || params.distanceKm <= 0 ||
        params.weightKg <= 0) {
        return false;
    }
    return true;
}