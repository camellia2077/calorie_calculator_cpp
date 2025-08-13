#include "CalculationEngine.h"

WorkoutResults CalculationEngine::calculate(const WorkoutParameters& params, const std::vector<DataPoint>& dataTable) const {
    // 1. 调用配速计算器，获取基础运动指标
    PaceCalculatorResult paceResult = paceCalculator_.calculate(params);

    // 2. 调用热量计算器，获取生理相关指标
    MetCalorieResult metResult = metCalorieCalculator_.calculate(
        paceResult.userSpeedKmh,
        params.weightKg,
        paceResult.totalTimeInMinutes,
        dataTable
    );

    // 3. 将所有子计算器的结果组合成一个统一的最终结果结构体
    WorkoutResults finalResults = {
        paceResult.totalTimeInMinutes,
        paceResult.userSpeedKmh,
        paceResult.speedMps,
        paceResult.paceMinutes,
        paceResult.paceSeconds,
        metResult.averageMets,
        metResult.totalKcal,
        metResult.totalKj,
        metResult.equivalentHours,
        metResult.equivalentMinutes
    };
    
    return finalResults;
}