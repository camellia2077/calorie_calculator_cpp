#include "PaceCalculator.h"
#include <cmath> // For fmod

PaceCalculatorResult PaceCalculator::calculate(const WorkoutParameters& params) const {
    PaceCalculatorResult results = {};

    // 1. 单位统一：将所有用户输入转换为国际单位制 (米 和 秒)
    const double totalMeters = params.distanceKm * 1000.0;
    const double totalSeconds = (params.timeHr * 3600.0) + (params.timeMin * 60.0) + params.timeSec;

    // 2. 核心计算：完全使用国际单位制
    const double speedInMps = (totalSeconds > 0) ? (totalMeters / totalSeconds) : 0.0;
    results.speedMps = speedInMps;

    // 3. 派生用于展示和其他模块的单位
    results.userSpeedKmh = speedInMps * 3.6;
    results.totalTimeInMinutes = totalSeconds / 60.0;

    // 4. (*** 核心修改：使用最高精度的方法计算配速 ***)
    if (params.distanceKm > 0) {
        // 直接用总秒数除以总公里数，得到每公里用时多少秒（带小数）
        const double secondsPerKm = totalSeconds / params.distanceKm;
        
        // 从总秒数中，取出整数分钟部分
        results.paceMinutes = static_cast<int>(secondsPerKm / 60.0);
        
        // 使用fmod函数，取出余下的、带小数的秒数部分
        results.paceSeconds = fmod(secondsPerKm, 60.0);
    } else {
        results.paceMinutes = 0;
        results.paceSeconds = 0.0;
    }
    
    return results;
}