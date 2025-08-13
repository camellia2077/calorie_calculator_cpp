#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <vector>
#include "domain/DataPoint.h" // 存储一对数据：speedKph (速度，单位是千米/小时) 和 metValue (METs值)

// 这是一个可复用的工具，负责执行线性插值计算

namespace Interpolator {

/**
 * @brief 根据用户速度，通过线性插值计算一个值（如METs）。
 * @param userSpeed 用户的速度。
 * @param dataTable 一个包含(速度, 值)数据点的有序向量。
 * @return double 插值计算后的精确值。
 */
double getInterpolatedValue(double userSpeed, const std::vector<DataPoint>& dataTable);

} // namespace Interpolator

#endif // INTERPOLATOR_H