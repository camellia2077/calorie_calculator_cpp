#ifndef PACE_CALCULATOR_H
#define PACE_CALCULATOR_H

#include "domain/WorkoutDomain.h" // 依赖于WorkoutParameters

// 这个类负责计算所有基础的运动表现指标
// 定义一个专门用于存储配速相关计算结果的结构体
struct PaceCalculatorResult {
    double totalTimeInMinutes;
    double userSpeedKmh;
    double speedMps;
    int paceMinutes;
    int paceSeconds;
};

class PaceCalculator {
public:
    /**
     * @brief 计算所有基础运动表现指标。
     * @param params 包含用户原始输入的结构体。
     * @return PaceCalculatorResult 包含所有计算好的配速相关结果。
     */
    PaceCalculatorResult calculate(const WorkoutParameters& params) const;
};

#endif // PACE_CALCULATOR_H