#ifndef FOOD_EQUIVALENT_H
#define FOOD_EQUIVALENT_H

#include <string>

// 用于存储一项食物的等效换算结果
struct FoodEquivalent {
    std::string name;           // 食物名称
    double amount;              // 换算出的数量
    std::string unit;           // 单位 (克 或 毫升)
    double kcalPer100;          // 该食物每100g/ml的热量
};

#endif // FOOD_EQUIVALENT_H