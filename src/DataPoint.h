#ifndef DATAPOINT_H
#define DATAPOINT_H

// 定义一个通用的数据点结构体
// 任何基于速度的运动都可以使用这个结构来存储其METs数据
struct DataPoint {
    double speedKph; // 速度 (km/h)
    double metValue; // METs值
};

#endif // DATAPOINT_H