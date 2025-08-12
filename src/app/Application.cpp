#include "Application.h"
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

    // 验证输入的合法性
    if (!validateParameters(params)) {
        ui.displayError("输入值不合法。时间不能为负，分钟和秒数需小于60，距离和体重必须为正数。");
        return;
    }

    // 在传递给引擎之前，先进行初步计算和验证
    double totalTimeInMinutes = (params.timeHr * 60.0) + params.timeMin + (params.timeSec / 60.0);
    double userSpeedKmh = (totalTimeInMinutes > 0) ? (params.distanceKm / (totalTimeInMinutes / 60.0)) : 0;
    
    // 验证速度是否在计算范围内
    if (userSpeedKmh < dataTable.front().speedKph || userSpeedKmh > dataTable.back().speedKph) {
        ui.displaySpeedError(userSpeedKmh, dataTable.front().speedKph, dataTable.back().speedKph);
        return;
    }

    // 调用计算引擎进行核心计算
    WorkoutResults results = engine.calculate(params, dataTable);

    // 显示结果
    ui.displayResults(results, activityName);
}

bool Application::validateParameters(const WorkoutParameters& params) {
    if (params.timeHr < 0 || params.timeMin < 0 || params.timeMin >= 60 || 
        params.timeSec < 0 || params.timeSec >= 60 || params.distanceKm <= 0 || 
        params.weightKg <= 0) {
        return false;
    }
    return true;
}