#ifndef FOOD_DATA_H
#define FOOD_DATA_H

#include <string>
#include <vector>

// 定义一个食物数据结构体
struct FoodData {
    std::string name;       // 食物名称
    double kcalPer100g;   // 每100克或100毫升所含的热量（千卡/大卡）
};

// --- 数据来源为通用营养数据库及品牌公开数据，实际数值可能因批次和地区而异 ---

// --- 常见水果 ---
const std::vector<FoodData> fruitData = {
    {"苹果", 52.0}, {"香蕉", 89.0}, {"西瓜", 30.0},
    {"草莓", 32.0}, {"橙子", 47.0}, {"芒果", 60.0},
};

// --- 常见蔬菜 (生重) ---
const std::vector<FoodData> vegetableData = {
    {"西兰花", 34.0}, {"黄瓜", 15.0}, {"番茄", 18.0}, {"胡萝卜", 41.0},
    {"菠菜", 23.0}, {"生菜", 15.0}
};

// --- 常见主食 (熟重) ---
const std::vector<FoodData> stapleFoodData = {
    {"米饭", 130.0}, {"馒头", 223.0}, {"面条 (煮熟)", 131.0},
    {"全麦面包", 247.0}, {"燕麦片 (加水煮)", 71.0}
};

// --- 常见肉类/蛋白质 (熟重) ---
const std::vector<FoodData> proteinData = {
    {"鸡胸肉 (去皮煮熟)", 165.0}, {"三文鱼 (烤)", 208.0}, {"牛肉 (瘦肉, 烤)", 250.0},
    {"猪肉 (里脊, 烤)", 143.0}, {"鸡蛋 (煮)", 155.0}, {"豆腐", 76.0}
};

// --- 常见零食 ---
const std::vector<FoodData> snackData = {
    {"乐事原味薯片", 543.0},
    {"奥利奥原味饼干", 478.0},
    {"趣多多原味曲奇", 507.0}
};

// --- 常见饮料 (热量单位: 千卡/100毫升) ---
const std::vector<FoodData> beverageData = {
    {"可口可乐", 43.0},
    {"百事可乐", 45.0},
    {"雪碧", 46.0}
};

#endif // FOOD_DATA_H