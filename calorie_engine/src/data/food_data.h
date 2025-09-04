#ifndef FOOD_DATA_H
#define FOOD_DATA_H

#include <string>
#include <vector>

// 定义食物的物理类型
enum class FoodType {
    Solid,  // 固体
    Liquid, // 液体
    Unknown // 新增：用于处理解析错误
};

// 基础数据结构：定义单个食物
struct FoodData {
    std::string name;       // 食物名称
    double kcalPer100;      // 每100g或100ml的热量
    FoodType type;          // 食物的物理类型
};

// 定义一个完整的食物类别
struct FoodCategoryData {
    std::string categoryName;
    std::vector<FoodData> items;
};

#endif // FOOD_DATA_H