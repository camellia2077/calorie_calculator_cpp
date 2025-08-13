#include "PaceCalculator.h"
#include <cmath> // For std::round

PaceCalculatorResult PaceCalculator::calculate(const WorkoutParameters& params) const {
    PaceCalculatorResult results = {};

    // --- 1. 单位统一：将所有用户输入转换为国际单位制 (米 和 秒) ---
    const double totalMeters = params.distanceKm * 1000.0;
    const double totalSeconds = (params.timeHr * 3600.0) + (params.timeMin * 60.0) + params.timeSec;

    // --- 2. 核心计算：完全使用国际单位制进行最基础的物理计算 ---
    const double speedInMps = (totalSeconds > 0) ? (totalMeters / totalSeconds) : 0.0;
    results.speedMps = speedInMps; // 直接存储核心计算结果

    // --- 3. 派生数据：根据核心计算结果，生成其他模块或UI需要的、不同单位的数据 ---

    // a. 为其他模块（如 MetCalorieCalculator）生成兼容的 km/h 单位
    results.userSpeedKmh = speedInMps * 3.6;

    // b. 为其他模块（如 MetCalorieCalculator）生成兼容的分钟单位
    results.totalTimeInMinutes = totalSeconds / 60.0;

    // c. 为UI生成配速（分'秒''/公里）
    if (results.userSpeedKmh > 0) {
        const double decimalMinutesPerKm = 60.0 / results.userSpeedKmh;
        results.paceMinutes = static_cast<int>(decimalMinutesPerKm);
        const double remainingFraction = decimalMinutesPerKm - results.paceMinutes;
        results.paceSeconds = static_cast<int>(round(remainingFraction * 60.0));
    } else {
        results.paceMinutes = 0;
        results.paceSeconds = 0;
    }
    
    return results;
}