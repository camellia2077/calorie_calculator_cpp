#include "app/Application.h"
#include "common/version.h" // Corrected path
#include "data/running_data.h"
#include "data/cycling_data.h"
#include <string>
#include <vector>

void Application::run() {
    while (true) {
        int choice = ui.getSportChoice(); 

        switch (choice) {
            case 0: {
                ui.displayVersionInfo(AppVersion::getVersionString(), AppVersion::LAST_UPDATED);
                break;
            }
            case 1: // 跑步
            case 2: // 骑行
                processWorkout(choice);
                break;
            case 3: // 假设选项3为退出
                ui.displayGoodbyeMessage();
                return; // 结束 run() 函数，退出程序
            default:
                ui.displayError("无效的选项，请重新输入。"); 
                break;
        }
    }
}

void Application::processWorkout(int sportChoice) {
    // 根据选择确定运动名称和数据表
    std::string activityName = (sportChoice == 1) ? "跑步" : "骑行";
    const auto& dataTable = (sportChoice == 1) ? runningData : cyclingData;

    // 获取用户输入
    WorkoutParameters params = ui.getWorkoutParameters(activityName);

    // 1. 验证输入的合法性
    if (!validateParameters(params)) {
        ui.displayError("输入值不合法。时间不能为负，分钟和秒数需小于60，距离和体重必须为正数。");
        return;
    }

    // 2. 将参数传递给计算引擎
    WorkoutResults results = engine.calculate(params, dataTable);
    
    // 3. 验证速度是否在计算范围内
    if (!dataTable.empty() && (results.userSpeedKmh < dataTable.front().speedKph || results.userSpeedKmh > dataTable.back().speedKph)) {
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