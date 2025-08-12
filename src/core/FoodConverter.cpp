
#include "FoodConverter.h"
#include "data/food_data.h" // 引用食物数据
#include <algorithm>

// 将计算逻辑从 ConsoleUI 移动到这里
std::map<std::string, std::vector<FoodEquivalent>> FoodConverter::calculate(double totalKcal) {
    std::map<std::string, std::vector<FoodEquivalent>> allEquivalents;

    auto processCategory = [&](const std::string& categoryName, const std::vector<FoodData>& foods, const std::string& unit) {
        std::vector<FoodEquivalent> categoryEquivalents;
        for (const auto& food : foods) {
            if (food.kcalPer100g > 0) {
                double equivalentAmount = totalKcal / (food.kcalPer100g / 100.0);
                categoryEquivalents.push_back({food.name, equivalentAmount, unit, food.kcalPer100g});
            }
        }
        // 按热量排序
        std::sort(categoryEquivalents.begin(), categoryEquivalents.end(), [](const FoodEquivalent& a, const FoodEquivalent& b) {
            return a.kcalPer100 < b.kcalPer100;
        });
        allEquivalents[categoryName] = categoryEquivalents;
    };

    processCategory("常见水果", fruitData, "克");
    processCategory("常见蔬菜", vegetableData, "克");
    processCategory("常见主食", stapleFoodData, "克");
    processCategory("常见肉类/蛋白质", proteinData, "克");
    processCategory("常见零食", snackData, "克");
    processCategory("常见饮料", beverageData, "毫升");

    return allEquivalents;
}