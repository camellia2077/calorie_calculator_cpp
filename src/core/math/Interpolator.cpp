#include "Interpolator.h"

// 线性插值计算的实现
double Interpolator::getInterpolatedValue(double userSpeed, const std::vector<DataPoint>& dataTable) {
    // 处理边界情况：速度超出数据表范围
    if (userSpeed >= dataTable.back().speedKph) {
        return dataTable.back().metValue;
    }
    if (userSpeed <= dataTable.front().speedKph) {
        return dataTable.front().metValue;
    }
    
    // 循环查找速度所在的区间
    for (size_t i = 0; i < dataTable.size() - 1; ++i) {
        if (userSpeed >= dataTable[i].speedKph && userSpeed < dataTable[i + 1].speedKph) {
            // 找到区间，执行线性插值
            const double x1 = dataTable[i].speedKph;
            const double y1 = dataTable[i].metValue;
            const double x2 = dataTable[i+1].speedKph;
            const double y2 = dataTable[i+1].metValue;
            return y1 + (userSpeed - x1) * (y2 - y1) / (x2 - x1);
        }
    }

    // 理论上，如果数据表有效，不应到达这里
    return 0.0; 
}