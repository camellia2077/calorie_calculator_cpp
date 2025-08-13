#include "PaceCalculator.h"
#include <cmath> // For std::round

PaceCalculatorResult PaceCalculator::calculate(const WorkoutParameters& params) const {
    PaceCalculatorResult results = {};

    // 1. 计算总时间
    results.totalTimeInMinutes = (params.timeHr * 60.0) + params.timeMin + (params.timeSec / 60.0);
    
    // 2. 计算速度 (km/h 和 m/s)
    results.userSpeedKmh = (results.totalTimeInMinutes > 0) 
                         ? (params.distanceKm / (results.totalTimeInMinutes / 60.0)) 
                         : 0;
    results.speedMps = results.userSpeedKmh / 3.6;

    // 3. 计算配速
    if (results.userSpeedKmh > 0) {
        const double paceDecimal = 60.0 / results.userSpeedKmh;
        results.paceMinutes = static_cast<int>(paceDecimal);
        results.paceSeconds = static_cast<int>(round((paceDecimal - results.paceMinutes) * 60.0));
    }
    
    return results;
}