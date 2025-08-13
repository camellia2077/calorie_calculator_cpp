#include "PaceCalculator.h"
#include <cmath>
// Public method - now acts as a coordinator
PaceCalculatorResult PaceCalculator::calculate(const WorkoutParameters& params) const {
    PaceCalculatorResult results = {};

    const double totalMeters = params.distanceKm * 1000.0;
    const double totalSeconds = calculateTotalSeconds(params);

    // Core calculation
    results.speedMps = (totalSeconds > 0) ? (totalMeters / totalSeconds) : 0.0;
    results.userSpeedKmh = results.speedMps * 3.6;
    results.totalTimeInMinutes = totalSeconds / 60.0;

    // Delegate pace calculation to a private helper
    calculatePaceComponents(totalSeconds, params.distanceKm, results.paceMinutes, results.paceSeconds);
    
    return results;
}

// Private helper for time conversion
double PaceCalculator::calculateTotalSeconds(const WorkoutParameters& params) const {
    return (params.timeHr * 3600.0) + (params.timeMin * 60.0) + params.timeSec;
}


// Private helper for pace calculation
void PaceCalculator::calculatePaceComponents(double totalSeconds, double distanceKm, int& outPaceMinutes, double& outPaceSeconds) const {
    if (distanceKm > 0) {
        const double secondsPerKm = totalSeconds / distanceKm;
        // 将浮点数强制转换为整数，从而只保留分钟的整数部分
        // 如果 secondsPerKm 是 330.5 秒，330.5 / 60.0 得到 5.5083，static_cast<int> 会将其截断为 5
        outPaceMinutes = static_cast<int>(secondsPerKm / 60.0);
        // 使用 fmod 保留秒数的小数部分
        // 如果 secondsPerKm 是 330.5 秒，fmod(330.5, 60.0) 会得到 30.5
        outPaceSeconds = fmod(secondsPerKm, 60.0);
    } else {
        outPaceMinutes = 0;
        outPaceSeconds = 0.0;
    }
}