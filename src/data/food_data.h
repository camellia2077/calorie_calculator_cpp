#ifndef FOOD_DATA_H
#define FOOD_DATA_H

#include <string>
#include <vector>

// 基础数据结构：定义单个食物
struct FoodData {
    std::string name;       // 食物名称
    double kcalPer100g;   // 每100克或毫升的热量
};

// ==================== 修改开始：引入新的顶层结构 ====================
// 新结构：定义一个完整的食物类别，包含名称、单位和食物列表
struct FoodCategoryData {
    std::string categoryName;
    std::string unit;
    std::vector<FoodData> items;
};

// --- 将所有食物数据统一到一个结构化向量中 ---

// 为了保持定义清晰，我们仍然可以先独立定义每个列表
const std::vector<FoodData> fruitItems = {
    {"苹果", 52.0}, {"香蕉", 89.0}, {"西瓜", 30.0}, {"草莓", 32.0}, {"橙子", 47.0}, {"芒果", 60.0}
};
const std::vector<FoodData> vegetableItems = {
    {"西兰花", 34.0}, {"黄瓜", 15.0}, {"番茄", 18.0}, {"胡萝卜", 41.0}, {"菠菜", 23.0}, {"生菜", 15.0}
};
const std::vector<FoodData> stapleItems = {
    {"米饭", 130.0}, {"馒头", 223.0}, {"面条 (煮熟)", 131.0}, {"全麦面包", 247.0}, {"燕麦片 (加水煮)", 71.0}
};
const std::vector<FoodData> proteinItems = {
    {"鸡胸肉 (去皮煮熟)", 165.0}, {"三文鱼 (烤)", 208.0}, {"牛肉 (瘦肉, 烤)", 250.0}, {"鸡蛋 (煮)", 155.0}
};
const std::vector<FoodData> snackItems = {
    {"乐事原味薯片", 543.0}, {"奥利奥原味饼干", 478.0}, {"趣多多原味曲奇", 507.0}
};
const std::vector<FoodData> beverageItems = {
    {"可口可乐", 43.0}, {"百事可乐", 45.0}, {"雪碧", 46.0}
};
const std::vector<FoodData> nutrientItems = {
    {"纯碳水化合物", 400.0}, {"纯蛋白质", 400.0}, {"纯脂肪", 900.0}
};

// 最终导出的单一数据源
const std::vector<FoodCategoryData> allFoodData = {
    {"常见水果", "克", fruitItems},
    {"常见蔬菜", "克", vegetableItems},
    {"常见主食", "克", stapleItems},
    {"常见肉类/蛋白质", "克", proteinItems},
    {"常见零食", "克", snackItems},
    {"常见饮料", "毫升", beverageItems},
    {"宏量营养素", "克", nutrientItems}
};
// ==================== 修改结束 ====================

#endif // FOOD_DATA_H