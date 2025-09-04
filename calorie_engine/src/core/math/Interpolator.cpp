#include "Interpolator.h"
#include <algorithm> // 包含 std::lower_bound
#include <iterator>  // 包含 std::distance

// 传入速度，传出对应的 METs 值
double Interpolator::getInterpolatedValue(double userSpeed, const std::vector<DataPoint>& dataTable) {
    // 1. 处理边界情况 (保持不变)
    if (dataTable.empty()) {
        return 0.0;
    }
    if (userSpeed >= dataTable.back().speedKph) {
        return dataTable.back().metValue;
    }
    if (userSpeed <= dataTable.front().speedKph) {
        return dataTable.front().metValue;
    }

    // 2. 使用 std::lower_bound 高效查找插值区间的右边界
    //    我们需要提供一个自定义比较函数（使用 lambda 表达式），
    //    告诉 lower_bound 如何比较一个 DataPoint 和一个 double (userSpeed)。
    auto it = std::lower_bound(dataTable.begin(), dataTable.end(), userSpeed,
        [](const DataPoint& point, double speed) {
            return point.speedKph < speed;
        });

    // std::lower_bound 找到的 'it' 是区间的右边界点 (x2, y2)。
    // 区间的左边界点 (x1, y1) 就是它前面的那个元素。
    // 因为我们已经处理了 userSpeed <= dataTable.front().speedKph 的情况，
    // 所以 'it' 不可能等于 dataTable.begin()，it - 1 的访问是安全的。
    // 迭代器转化为索引
    size_t index2 = std::distance(dataTable.begin(), it);
    size_t index1 = index2 - 1;

    // 3. 执行线性插值
    const double x1 = dataTable[index1].speedKph;
    const double y1 = dataTable[index1].metValue;
    const double x2 = dataTable[index2].speedKph;
    const double y2 = dataTable[index2].metValue;
    
    // 4. 处理垂直线的情况，即斜率不存在
    if (x2 == x1) {
        return y1;
    }
    // 5. 用一个清晰的变量名来存储斜率的计算结果。
    const double slope = (y2 - y1) / (x2 - x1);

    // 6. 使用这个斜率变量来计算最终结果。
    // 这个公式 y = y1 + (x - x1) * slope 是线性插值的标准点斜式方程。
    const double interpolatedMet = y1 + (userSpeed - x1) * slope;

return interpolatedMet;
}