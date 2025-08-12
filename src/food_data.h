#ifndef FOOD_DATA_H
#define FOOD_DATA_H

#include <string>
#include <vector>

// 定义一个食物数据结构体
struct FoodData {
    std::string name;       // 食物名称
    double kcalPer100g;   // 每100克所含的热量（千卡/大卡）
};

// 将所有食物热量数据存放在一个向量中
// 数据来源为通用营养数据库的平均值
const std::vector<FoodData> foodCalorieData = {
    {"苹果", 52.0},   // 苹果大约每100克含52千卡
    {"香蕉", 89.0},   // 香蕉大约每100克含89千卡
    {"西瓜", 30.0}    // 西瓜大约每100克含30千卡
};

#endif // FOOD_DATA_H