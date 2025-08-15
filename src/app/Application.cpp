#include "app/Application.h"
#include "common/version.h"
#include "data/running_data.h"
#include "data/cycling_data.h"
#include <string>
#include <vector>

// 交互式主循环
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
            case 3: // 退出
                ui.displayGoodbyeMessage();
                return;
            default:
                ui.displayError("无效的选项，请重新输入。");
                break;
        }
    }
}

// 从UI获取参数并启动单次处理
void Application::processWorkout(int sportChoice) {
    std::string sportType = (sportChoice == 1) ? "run" : "bike";
    std::string activityName = (sportChoice == 1) ? "跑步" : "骑行";
    
    // 获取用户输入
    WorkoutParameters params = ui.getWorkoutParameters(activityName);

    // 调用新的核心处理函数
    runOnce(sportType, params);
}

// 新的核心业务流程，可被外部直接调用
void Application::runOnce(const std::string& sportType, const WorkoutParameters& params) {
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
    auto foodEquivalents = foodConverter.calculate(results.totalKcal);

    // 6. 将所有结果传递给UI进行显示
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