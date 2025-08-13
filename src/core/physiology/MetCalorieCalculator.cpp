#include "MetCalorieCalculator.h"
#include "core/math/Interpolator.h" // 依赖于插值工具
#include <cmath> // For std::round and fmod

MetCalorieResult MetCalorieCalculator::calculate(double userSpeedKmh, double weightKg, double totalTimeInMinutes, const std::vector<DataPoint>& dataTable) const {
    MetCalorieResult results = {};

    // 1. 计算平均METs (调用通用插值工具)
    results.averageMets = Interpolator::getInterpolatedValue(userSpeedKmh, dataTable);
    
    // 2. 计算热量消耗 (千卡和千焦)
    const double totalTimeInHours = totalTimeInMinutes / 60.0;
    results.totalKcal = results.averageMets * weightKg * totalTimeInHours;
    
    const double KJ_PER_KCAL = 4.184;
    results.totalKj = results.totalKcal * KJ_PER_KCAL;
    
    // 3. 计算等效静坐活动
    const double equivalentRestingMinutes = results.averageMets * totalTimeInMinutes;
    results.equivalentHours = static_cast<int>(equivalentRestingMinutes / 60);
    results.equivalentMinutes = static_cast<int>(round(fmod(equivalentRestingMinutes, 60.0)));
    
    return results;
}