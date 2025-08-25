#include "FoodConverter.h"
#include "data/food_data.h" // 引用食物数据
#include <algorithm>

std::map<std::string, std::vector<FoodEquivalent>> FoodConverter::calculate(double totalKcal) {
    std::map<std::string, std::vector<FoodEquivalent>> allEquivalents;

    // 遍历统一的食物数据源
    for (const auto& category : allFoodData) {
        // 调用已更新的 processCategory 函数，不再传递单位
        processCategory(
            totalKcal,
            category.categoryName,
            category.items,
            allEquivalents
        );
    }

    return allEquivalents;
}

void FoodConverter::processCategory(
    double totalKcal,
    const std::string& categoryName,
    const std::vector<FoodData>& foods,
    std::map<std::string, std::vector<FoodEquivalent>>& allEquivalents) {
    
    std::vector<FoodEquivalent> categoryEquivalents;

    for (const auto& food : foods) {
        if (food.kcalPer100 > 0) {
            // ==================== 修改核心 ====================
            // 1. 根据食物类型决定单位
            std::string unit = (food.type == FoodType::Solid) ? "克" : "毫升";
            
            // 2. 计算等效数量
            double equivalentAmount = totalKcal / (food.kcalPer100 / 100.0);

            // 3. 将包含正确单位的结果存入列表
            categoryEquivalents.push_back({food.name, equivalentAmount, unit, food.kcalPer100});
            // ===============================================
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