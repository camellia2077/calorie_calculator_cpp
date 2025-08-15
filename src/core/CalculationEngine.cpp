#include "CalculationEngine.h"

WorkoutResults CalculationEngine::calculate(const WorkoutParameters& params, const std::vector<DataPoint>& dataTable) const {
    // 1. 调用配速计算器，获取基础运动指标
    PaceCalculatorResult paceResult = paceCalculator_.calculate(params);

    // 2. 调用热量计算器，计算代谢当量
    MetCalorieResult metResult = metCalorieCalculator_.calculate(
        paceResult.userSpeedKmh,
        params.weightKg,
        paceResult.totalTimeInMinutes,
        dataTable
    );

    // 3. 将所有子计算器的结果组合成一个统一的最终结果结构体
    WorkoutResults finalResults = {
        paceResult.totalTimeInMinutes, // 总运动时间，以分钟为单位
        paceResult.userSpeedKmh, // 平均速度 km/h
        paceResult.speedMps,// 速度m/s
        paceResult.paceMinutes, // 平均配速的分钟部分
        paceResult.paceSeconds, // 平均配速的秒数部分
        metResult.averageMets, // 平均代谢当量
        metResult.totalKcal, // 总热量 kcal
        metResult.totalKj, // 总热量 kJ
        metResult.equivalentHours, // 等效静坐活动时间的小时部分
        metResult.equivalentMinutes // 等效静坐活动时间的分钟部分
    };
    
    return finalResults;
}