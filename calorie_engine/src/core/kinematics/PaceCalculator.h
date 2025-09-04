#ifndef PACE_CALCULATOR_H
#define PACE_CALCULATOR_H

#include "domain/WorkoutDomain.h"

// 定义一个专门用于存储配速相关计算结果的结构体
struct PaceCalculatorResult {
    /**
     * @brief 总运动时间，以分钟为单位。例如：1小时30分30秒 -> 90.5
     * 
     */
    double totalTimeInMinutes;

    /**
     * @brief 用户的平均速度，单位为“公里/小时 (km/h)”。
     * 这个单位主要用于UI显示和某些特定的生理计算。
     */
    double userSpeedKmh;

    /**
     * @brief 用户的平均速度，单位为国际单位制的“米/秒 (m/s)”。
     * 这是程序内部进行核心物理计算时使用的标准单位。
     */
    double speedMps;

    /**
     * @brief 平均配速的“分钟”部分。
     * 例如：如果配速是 5'30.5''/km，这个值就是 5。
     */
    int paceMinutes;

    /**
     * @brief 平均配速的“秒数”部分，使用 double 类型以保留小数，提供更高精度。
     * 例如：如果配速是 5'30.5''/km，这个值就是 30.5。
     */
    double paceSeconds;
};

class PaceCalculator {
public:
    // The public interface remains the same
    PaceCalculatorResult calculate(const WorkoutParameters& params) const;

private:
    // Private helper functions to encapsulate logic
    double calculateTotalSeconds(const WorkoutParameters& params) const;
    void calculatePaceComponents(double totalSeconds, double distanceKm, int& outPaceMinutes, double& outPaceSeconds) const;
};

#endif // PACE_CALCULATOR_H