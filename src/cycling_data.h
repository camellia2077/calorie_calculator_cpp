#ifndef CYCLING_DATA_H
#define CYCLING_DATA_H

#include <vector>
#include "DataPoint.h" // 包含数据点结构的定义

// 将所有骑行数据存放在一个结构体向量中
const std::vector<DataPoint> cyclingData = {
    {8.85, 3.5}, {15.13, 5.8}, {17.62, 6.8}, {20.84, 8.0},
    {24.06, 10.0}, {28.16, 12.0}, {32.19, 16.8}
};

#endif // CYCLING_DATA_H