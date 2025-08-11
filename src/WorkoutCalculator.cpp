#include "WorkoutCalculator.h"
#include <iostream>
#include <iomanip>
#include <cmath>

// 构造函数的实现
WorkoutCalculator::WorkoutCalculator(const std::string& name, const std::vector<DataPoint>& data)
    : activityName(name), dataTable(data) {}

// 主流程函数的实现
void WorkoutCalculator::run() {
    getUserInput();
    if (calculateResults()) {
        displayResults();
    }
}

// 获取用户输入的实现
void WorkoutCalculator::getUserInput() {
    std::cout << "请输入您的" << activityName << "时间（小时部分）: ";
    std::cin >> currentWorkout.timeHr;
    std::cout << "请输入您的" << activityName << "时间（分钟部分）: ";
    std::cin >> currentWorkout.timeMin;
    std::cout << "请输入您的" << activityName << "时间（秒的部分）: ";
    std::cin >> currentWorkout.timeSec;

    std::cout << "请输入您的" << activityName << "距离（公里）: ";
    std::cin >> currentWorkout.distanceKm;

    std::cout << "请输入您的体重（公斤）: ";
    std::cin >> currentWorkout.weightKg;
}

// 执行计算的实现
bool WorkoutCalculator::calculateResults() {
    auto& data = currentWorkout; // 使用别名简化代码

    if (data.timeHr < 0 || data.timeMin < 0 || data.timeMin >= 60 || data.timeSec < 0 || data.timeSec >= 60 || data.distanceKm <= 0 || data.weightKg <= 0) {
        std::cerr << "错误：输入值不合法。" << std::endl;
        return false;
    }

    data.totalTimeInMinutes = (data.timeHr * 60.0) + data.timeMin + (data.timeSec / 60.0);
    data.userSpeedKmh = (data.totalTimeInMinutes > 0) ? (data.distanceKm / (data.totalTimeInMinutes / 60.0)) : 0;
    
    if (data.userSpeedKmh < dataTable.front().speedKph || data.userSpeedKmh > dataTable.back().speedKph) {
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "您的平均速度是: " << data.userSpeedKmh << " 公里/小时 (km/h)" << std::endl;
        std::cout << "\n错误：您的速度不在有效计算范围内。" << std::endl;
        std::cout << "本程序可计算的速度范围为 " << dataTable.front().speedKph << " 到 " << dataTable.back().speedKph << " 公里/小时。" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return false;
    }

    data.speedMps = data.userSpeedKmh / 3.6;
    double paceDecimal = (data.userSpeedKmh > 0) ? (60.0 / data.userSpeedKmh) : 0;
    data.paceMinutes = static_cast<int>(paceDecimal);
    data.paceSeconds = static_cast<int>(round((paceDecimal - data.paceMinutes) * 60.0));
    
    data.averageMets = getInterpolatedValue(data.userSpeedKmh);
    data.totalKcal = data.averageMets * data.weightKg * (data.totalTimeInMinutes / 60.0);
    const double KJ_PER_KCAL = 4.184;
    data.totalKj = data.totalKcal * KJ_PER_KCAL;
    
    double equivalentRestingMinutes = data.averageMets * data.totalTimeInMinutes;
    data.equivalentHours = static_cast<int>(equivalentRestingMinutes / 60);
    data.equivalentMinutes = static_cast<int>(round(fmod(equivalentRestingMinutes, 60.0)));
    
    return true;
}

// 显示结果的实现
void WorkoutCalculator::displayResults() {
    const auto& data = currentWorkout; // 使用 const 别名

    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "您的平均速度是: " << data.userSpeedKmh << " 公里/小时 (km/h)" << std::endl;
    std::cout << "               " << "(" << data.speedMps << " 米/秒 (m/s))" << std::endl;
    std::cout << "您的平均配速是: " << data.paceMinutes << "分" << data.paceSeconds << "秒 / 公里" << std::endl;
    std::cout << "\n计算结果:" << std::endl;
    std::cout << "本次" << activityName << "平均代谢当量 (METs): " << data.averageMets << std::endl;
    std::cout << "您消耗的总热量约为: " << data.totalKj << " 千焦 (kJ)" << std::endl;
    std::cout << "                  " << "或 " << data.totalKcal << " 千卡/大卡 (kcal)" << std::endl;
    std::cout << "\n从消耗热量来看，本次" << activityName << "相当于静坐了：" << std::endl;
    std::cout << data.equivalentHours << " 小时 " << data.equivalentMinutes << " 分钟" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

// 插值计算的实现
double WorkoutCalculator::getInterpolatedValue(double userSpeed) {
    if (userSpeed >= dataTable.back().speedKph) return dataTable.back().metValue;
    if (userSpeed <= dataTable.front().speedKph) return dataTable.front().metValue;
    
    for (size_t i = 0; i < dataTable.size() - 1; ++i) {
        if (userSpeed >= dataTable[i].speedKph && userSpeed < dataTable[i + 1].speedKph) {
            double x1 = dataTable[i].speedKph;
            double y1 = dataTable[i].metValue;
            double x2 = dataTable[i+1].speedKph;
            double y2 = dataTable[i+1].metValue;
            return y1 + (userSpeed - x1) * (y2 - y1) / (x2 - x1);
        }
    }
    return 0.0; // 理论上不会到达这里
}