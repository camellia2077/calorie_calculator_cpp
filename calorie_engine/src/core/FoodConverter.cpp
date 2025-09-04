#include "FoodConverter.h"
#include <algorithm>

// 修改后的 calculate 方法
std::map<std::string, std::vector<FoodEquivalent>> FoodConverter::calculate(
    double totalKcal, 
    const std::vector<FoodCategoryData>& allFoodData) { // 接收食物数据作为参数
    
    std::map<std::string, std::vector<FoodEquivalent>> allEquivalents;

    // 遍历从外部传入的食物数据源
    for (const auto& category : allFoodData) {
        // 调用私有辅助函数来处理每个类别，这部分逻辑保持不变
        processCategory(
            totalKcal,
            category.categoryName,
            category.items,
            allEquivalents
        );
    }

    return allEquivalents;
}

// processCategory 函数保持不变，它的逻辑已经是通用的
void FoodConverter::processCategory(
    double totalKcal,
    const std::string& categoryName,
    const std::vector<FoodData>& foods,
    std::map<std::string, std::vector<FoodEquivalent>>& allEquivalents) {
    
    std::vector<FoodEquivalent> categoryEquivalents;

    for (const auto& food : foods) {
        if (food.kcalPer100 > 0) {
            // 根据食物类型（固体/液体）决定单位是“克”还是“毫升”
            std::string unit = (food.type == FoodType::Solid) ? "克" : "毫升";
            
            // 计算等效的数量
            double equivalentAmount = totalKcal / (food.kcalPer100 / 100.0);

            // 将计算结果存入列表
            categoryEquivalents.push_back({food.name, equivalentAmount, unit, food.kcalPer100});
        }
    }

    // 对当前类别的食物按数量进行排序
    sortEquivalentsByAmount(categoryEquivalents);
    allEquivalents[categoryName] = categoryEquivalents;
}

// sortEquivalentsByAmount 函数保持不变
void FoodConverter::sortEquivalentsByAmount(std::vector<FoodEquivalent>& equivalents) {
    std::sort(equivalents.begin(), equivalents.end(), [](const FoodEquivalent& a, const FoodEquivalent& b) {
        return a.amount > b.amount;
    });
}