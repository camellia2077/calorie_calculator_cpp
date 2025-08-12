#ifndef CALCULATION_ENGINE_H
#define CALCULATION_ENGINE_H

#include <vector>
#include "DataPoint.h"
#include "WorkoutDomain.h"

class CalculationEngine {
public:
    // 执行所有核心计算，接收参数并返回结果
    WorkoutResults calculate(const WorkoutParameters& params, const std::vector<DataPoint>& dataTable) const;

private:
    // 私有辅助函数，用于线性插值计算METs
    double getInterpolatedValue(double userSpeed, const std::vector<DataPoint>& dataTable) const;
};

#endif // CALCULATION_ENGINE_H