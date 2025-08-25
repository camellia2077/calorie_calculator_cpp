#ifndef FOOD_DATA_H
#define FOOD_DATA_H

#include <string>
#include <vector>

// 新增：定义食物的物理类型
enum class FoodType {
    Solid,  // 固体
    Liquid  // 液体
};

// 基础数据结构：定义单个食物
struct FoodData {
    std::string name;       // 食物名称
    double kcalPer100;      // 每100g或100ml的热量
    FoodType type;          // 食物的物理类型
};

// 新结构：定义一个完整的食物类别
struct FoodCategoryData {
    std::string categoryName;
    std::vector<FoodData> items;
};

// --- 将所有食物数据统一到一个结构化向量中 ---

// 为了保持定义清晰，我们仍然可以先独立定义每个列表
const std::vector<FoodData> fruitItems = {
    {"苹果", 52.0, FoodType::Solid}, {"香蕉", 89.0, FoodType::Solid}, {"西瓜", 30.0, FoodType::Solid},
    {"草莓", 32.0, FoodType::Solid}, {"橙子", 47.0, FoodType::Solid}, {"芒果", 60.0, FoodType::Solid}
};
const std::vector<FoodData> vegetableItems = {
    {"西兰花", 34.0, FoodType::Solid}, {"黄瓜", 15.0, FoodType::Solid}, {"番茄", 18.0, FoodType::Solid},
    {"胡萝卜", 41.0, FoodType::Solid}, {"菠菜", 23.0, FoodType::Solid}, {"生菜", 15.0, FoodType::Solid}
};
const std::vector<FoodData> stapleItems = {
    {"米饭", 130.0, FoodType::Solid}, {"馒头", 223.0, FoodType::Solid}, {"面条 (煮熟)", 131.0, FoodType::Solid},
    {"全麦面包", 247.0, FoodType::Solid}, {"燕麦片 (加水煮)", 71.0, FoodType::Solid}
};
const std::vector<FoodData> proteinItems = {
    {"鸡胸肉 (去皮煮熟)", 165.0, FoodType::Solid}, {"三文鱼 (烤)", 208.0, FoodType::Solid},
    {"牛肉 (瘦肉, 烤)", 250.0, FoodType::Solid}, {"鸡蛋 (煮)", 155.0, FoodType::Solid}
};
const std::vector<FoodData> snackItems = {
    {"乐事原味薯片", 543.0, FoodType::Solid}, {"奥利奥原味饼干", 478.0, FoodType::Solid},
    {"趣多多原味曲奇", 507.0, FoodType::Solid}
};
const std::vector<FoodData> beverageItems = {
    {"可口可乐", 43.0, FoodType::Liquid}, {"百事可乐", 45.0, FoodType::Liquid}, {"雪碧", 46.0, FoodType::Liquid}
};
const std::vector<FoodData> nutrientItems = {
    {"纯碳水化合物", 400.0, FoodType::Solid}, {"纯蛋白质", 400.0, FoodType::Solid}, {"纯脂肪", 900.0, FoodType::Solid}
};

// 最终导出的单一数据源 (不再包含单位)
const std::vector<FoodCategoryData> allFoodData = {
    {"常见水果", fruitItems},
    {"常见蔬菜", vegetableItems},
    {"常见主食", stapleItems},
    {"常见肉类/蛋白质", proteinItems},
    {"常见零食", snackItems},
    {"常见饮料", beverageItems},
    {"宏量营养素", nutrientItems}
};

#endif // FOOD_DATA_H