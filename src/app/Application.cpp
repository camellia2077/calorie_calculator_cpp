#include "app/Application.h"
#include "data/running_data.h"
#include "data/cycling_data.h"
#include <string>
#include <vector>

void Application::run() {
    int choice = ui.getSportChoice();
    
    // 根据选择确定运动名称和数据表
    std::string activityName = (choice == 1) ? "跑步" : "骑行";
    const auto& dataTable = (choice == 1) ? runningData : cyclingData;

    // 获取用户输入
    WorkoutParameters params = ui.getWorkoutParameters(activityName);

    // 1. 验证输入的合法性 (例如非负数等)
    if (!validateParameters(params)) {
        ui.displayError("输入值不合法。时间不能为负，分钟和秒数需小于60，距离和体重必须为正数。");
        return;
    }

    // 2. (*** 这是主要修改点 ***)
    //    现在直接将原始参数传递给计算引擎，不再进行任何预计算。
    //    由 CalculationEngine 内部的 PaceCalculator 来计算速度。
    WorkoutResults results = engine.calculate(params, dataTable);
    
    // 3. 验证速度是否在计算范围内
    //    我们从引擎返回的结果中获取速度来进行验证。
    if (results.userSpeedKmh < dataTable.front().speedKph || results.userSpeedKmh > dataTable.back().speedKph) {
        ui.displaySpeedError(results.userSpeedKmh, dataTable.front().speedKph, dataTable.back().speedKph);
        return;
    }

    // 4. 调用食物转换器
    auto foodEquivalents = foodConverter.calculate(results.totalKcal);

    // 5. 将所有结果传递给UI进行显示
    ui.displayResults(results, activityName, foodEquivalents);
}

bool Application::validateParameters(const WorkoutParameters& params) {
    if (params.timeHr < 0 || params.timeMin < 0 || params.timeMin >= 60 || 
        params.timeSec < 0 || params.timeSec >= 60 || params.distanceKm <= 0 || 
        params.weightKg <= 0) {
        return false;
    }
    return true;
}