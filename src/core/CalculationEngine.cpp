#include "common/pch.h"
#include "CalculationEngine.h"
#include <cmath>

// 执行所有计算
WorkoutResults CalculationEngine::calculate(const WorkoutParameters& params, const std::vector<DataPoint>& dataTable) const {
    WorkoutResults results = {};

    results.totalTimeInMinutes = (params.timeHr * 60.0) + params.timeMin + (params.timeSec / 60.0);
    
    results.userSpeedKmh = (results.totalTimeInMinutes > 0) 
                         ? (params.distanceKm / (results.totalTimeInMinutes / 60.0)) 
                         : 0;

    results.speedMps = results.userSpeedKmh / 3.6;

    double paceDecimal = (results.userSpeedKmh > 0) ? (60.0 / results.userSpeedKmh) : 0;
    results.paceMinutes = static_cast<int>(paceDecimal);
    results.paceSeconds = static_cast<int>(round((paceDecimal - results.paceMinutes) * 60.0));
    
    results.averageMets = getInterpolatedValue(results.userSpeedKmh, dataTable);
    results.totalKcal = results.averageMets * params.weightKg * (results.totalTimeInMinutes / 60.0);
    
    const double KJ_PER_KCAL = 4.184;
    results.totalKj = results.totalKcal * KJ_PER_KCAL;
    
    double equivalentRestingMinutes = results.averageMets * results.totalTimeInMinutes;
    results.equivalentHours = static_cast<int>(equivalentRestingMinutes / 60);
    results.equivalentMinutes = static_cast<int>(round(fmod(equivalentRestingMinutes, 60.0)));
    
    return results;
}

// 线性插值计算
double CalculationEngine::getInterpolatedValue(double userSpeed, const std::vector<DataPoint>& dataTable) const {
    if (userSpeed >= dataTable.back().speedKph) return dataTable.back().metValue;
    if (userSpeed <= dataTable.front().speedKph) return dataTable.front().metValue;
    
    for (size_t i = 0; i < dataTable.size() - 1; ++i) {
        if (userSpeed >= dataTable[i].speedKph && userSpeed < dataTable[i + 1].speedKph) {
            double x1 = dataTable[i].speedKph;
            double y1 = dataTable[i].metValue;
            double x2 = dataTable[i+1].speedKph;
            double y2 = dataTable[i+1].metValue;
            return y1 + (userSpeed - x1) * (y2 - y1) / (x2 - x1);
        }
    }
    return 0.0; 
}