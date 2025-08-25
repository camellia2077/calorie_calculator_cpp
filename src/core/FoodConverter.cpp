#include "FoodConverter.h"
#include "data/food_data.h" // 引用食物数据
#include <algorithm>

std::map<std::string, std::vector<FoodEquivalent>> FoodConverter::calculate(double totalKcal) {
    std::map<std::string, std::vector<FoodEquivalent>> allEquivalents;

    // ==================== 修改开始：逻辑大大简化 ====================
    // 遍历统一的食物数据源
    for (const auto& category : allFoodData) {
        // 直接从 category 对象中获取所有需要的信息
        processCategory(
            totalKcal,
            category.categoryName,
            category.items,
            category.unit,
            allEquivalents
        );
    }
    // ==================== 修改结束 ====================

    return allEquivalents;
}

void FoodConverter::processCategory(
    double totalKcal,
    const std::string& categoryName,
    const std::vector<FoodData>& foods,
    const std::string& unit,
    std::map<std::string, std::vector<FoodEquivalent>>& allEquivalents) {
    
    std::vector<FoodEquivalent> categoryEquivalents;

    for (const auto& food : foods) {
        if (food.kcalPer100g > 0) {
            double equivalentAmount = totalKcal / (food.kcalPer100g / 100.0);
            categoryEquivalents.push_back({food.name, equivalentAmount, unit, food.kcalPer100g});
        }
    }

    sortEquivalentsByAmount(categoryEquivalents);
    allEquivalents[categoryName] = categoryEquivalents;
}

void FoodConverter::sortEquivalentsByAmount(std::vector<FoodEquivalent>& equivalents) {
    std::sort(equivalents.begin(), equivalents.end(), [](const FoodEquivalent& a, const FoodEquivalent& b) {
        return a.amount > b.amount;
    });
}