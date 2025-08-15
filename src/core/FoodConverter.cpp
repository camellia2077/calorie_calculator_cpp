#include "FoodConverter.h"
#include "data/food_data.h" // 引用食物数据
#include <algorithm>

/**
 * @brief [公有] 主计算函数。
 * * 这是该类的入口点。它接收一个总热量，然后通过调用私有辅助函数
 * processCategory 来处理预定义的每一种食物类别，最终返回一个包含
 * 所有类别计算结果的map。
 */
std::map<std::string, std::vector<FoodEquivalent>> FoodConverter::calculate(double totalKcal) {
    // 1. 创建一个map用于存储最终结果，键为类别名称，值为该类别下的食物列表。
    std::map<std::string, std::vector<FoodEquivalent>> allEquivalents;

    // 2. 对每种食物类别，调用 processCategory 辅助函数进行处理。
    //    这里硬编码了要处理的食物类别和它们对应的数据源。
    processCategory(totalKcal, "常见水果", fruitData, "克", allEquivalents);
    processCategory(totalKcal, "常见蔬菜", vegetableData, "克", allEquivalents);
    processCategory(totalKcal, "常见主食", stapleFoodData, "克", allEquivalents);
    processCategory(totalKcal, "常见肉类/蛋白质", proteinData, "克", allEquivalents);
    processCategory(totalKcal, "常见零食", snackData, "克", allEquivalents);
    processCategory(totalKcal, "常见饮料", beverageData, "毫升", allEquivalents);

    // 3. 返回填充了所有数据的map。
    return allEquivalents;
}

/**
 * @brief [私有] 处理单个食物类别的辅助函数。
 *
 * 它的任务是：
 * 1. 遍历一个特定类别（如水果）的所有食物项。
 * 2. 对每一项食物，计算达到 totalKcal 所需的分量。
 * 3. 将计算结果存入一个临时向量。
 * 4. 对这个向量进行排序。
 * 5. 将排序后的向量存入最终结果的map中。
 */
void FoodConverter::processCategory(
    double totalKcal,
    const std::string& categoryName,
    const std::vector<FoodData>& foods,
    const std::string& unit,
    std::map<std::string, std::vector<FoodEquivalent>>& allEquivalents) {
    
    // 创建一个临时向量，用于存放当前类别的计算结果。
    std::vector<FoodEquivalent> categoryEquivalents;

    // 遍历该类别中的每一种食物。
    for (const auto& food : foods) {
        // 安全检查：确保每100克/毫升的热量大于0，避免除以零的错误。
        if (food.kcalPer100g > 0) {
            // 计算公式：等效分量 = 总热量 / (每克或每毫升的热量)
            // 其中，每克或每毫升的热量 = 每100克或毫升的热量 / 100.0
            double equivalentAmount = totalKcal / (food.kcalPer100g / 100.0);
            
            // 将计算出的食物名称、等效分量、单位和原始热量数据存入向量。
            categoryEquivalents.push_back({food.name, equivalentAmount, unit, food.kcalPer100g});
        }
    }

    // 调用独立的排序函数，对当前类别的结果按分量进行降序排序。
    sortEquivalentsByAmount(categoryEquivalents);

    // 将处理完成的当前类别食物列表（categoryEquivalents）存入最终的map中。
    allEquivalents[categoryName] = categoryEquivalents;
}

/**
 * @brief [私有] 按分量对列表进行降序排序。
 * * 使用 C++ 标准库的 std::sort 算法。
 * 排序规则是基于食物的 amount 字段，进行降序排列（分量多的在前）。
 */
void FoodConverter::sortEquivalentsByAmount(std::vector<FoodEquivalent>& equivalents) {
    // 使用 lambda 表达式定义排序规则：比较两个 FoodEquivalent 对象的 amount 属性。
    // a.amount > b.amount 表示按降序排序。
    std::sort(equivalents.begin(), equivalents.end(), [](const FoodEquivalent& a, const FoodEquivalent& b) {
        return a.amount > b.amount;
    });
}