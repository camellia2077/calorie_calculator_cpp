#ifndef PACE_CALCULATOR_H
#define PACE_CALCULATOR_H

#include "domain/WorkoutDomain.h"

// 结果结构体保持不变，因为它定义了对外的“数据合约”
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
     * @brief 接收用户输入，内部使用国际单位制进行核心计算，并返回一个包含多种单位的结果。
     * @param params 包含用户原始输入的结构体 (km, h, min, s)。
     * @return PaceCalculatorResult 包含所有计算好的运动表现指标。
     */
    PaceCalculatorResult calculate(const WorkoutParameters& params) const;
};

#endif // PACE_CALCULATOR_H