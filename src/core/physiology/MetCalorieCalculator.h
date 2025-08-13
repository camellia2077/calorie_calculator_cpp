#ifndef MET_CALORIE_CALCULATOR_H
#define MET_CALORIE_CALCULATOR_H

#include <vector>
#include "domain/DataPoint.h"
// 这个类负责所有与生理相关的计算，它会使用 Interpolator 工具
// 定义一个专门用于存储生理相关计算结果的结构体
struct MetCalorieResult {
    double averageMets;
    double totalKcal;
    double totalKj;
    int equivalentHours;
    int equivalentMinutes;
};

class MetCalorieCalculator {
public:
    /**
     * @brief 计算所有生理相关指标。
     * @param userSpeedKmh 用户的平均速度 (km/h)，由PaceCalculator计算得出。
     * @param weightKg 用户的体重 (kg)。
     * @param totalTimeInMinutes 用户的总运动时间 (分钟)，由PaceCalculator计算得出。
     * @param dataTable 包含(速度, METs)数据点的向量。
     * @return MetCalorieResult 包含所有计算好的生理相关结果。
     */
    MetCalorieResult calculate(double userSpeedKmh, double weightKg, double totalTimeInMinutes, const std::vector<DataPoint>& dataTable) const;
};

#endif // MET_CALORIE_CALCULATOR_H