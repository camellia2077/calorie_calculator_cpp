#ifndef CYCLING_DATA_H
#define CYCLING_DATA_H

#include <vector>

// 1. 定义数据点结构体 (可以和跑步计算器共用相同的结构)
struct DataPoint {
    double speedKph; // 速度 (km/h)
    double metValue; // METs值
};

// 2. 将所有骑行数据存放在一个结构体向量中
// 数据来源: Compendium of Physical Activities (Bicycling)
const std::vector<DataPoint> cyclingData = {
    {8.85, 3.5},
    {15.13, 5.8},
    {17.62, 6.8},
    {20.84, 8.0},
    {24.06, 10.0},
    {28.16, 12.0},
    {32.19, 16.8}
};

#endif // CYCLING_DATA_H