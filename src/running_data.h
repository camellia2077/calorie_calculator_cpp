#ifndef RUNNING_DATA_H
#define RUNNING_DATA_H

#include <vector>
#include "DataPoint.h" // 包含数据点结构的定义

// 将所有跑步数据存放在一个结构体向量中
const std::vector<DataPoint> runningData = {
    {6.60, 6.5}, {7.32, 7.8}, {8.21, 8.5}, {9.09, 9.0}, {9.90, 9.3},
    {10.78, 10.5}, {11.27, 11.0}, {12.07, 11.8}, {12.87, 12.0},
    {13.84, 12.5}, {14.48, 13.0}, {15.21, 14.8}, {16.09, 14.8},
    {17.70, 16.8}, {19.31, 18.5}, {20.92, 19.8}, {22.53, 23.0}
};

#endif // RUNNING_DATA_H