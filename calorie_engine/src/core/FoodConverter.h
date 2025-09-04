#ifndef FOOD_CONVERTER_H
#define FOOD_CONVERTER_H

#include <vector>
#include <map>
#include <string>
#include "domain/FoodEquivalent.h"
#include "data/food_data.h" // 仅用于引用 FoodCategoryData 和 FoodData 结构体

/**
 * @class FoodConverter
 * @brief 一个用于将热量值转换为等效食物分量的工具类。
 * 此类现在是无状态的，其行为完全由传入的参数决定。
 */
class FoodConverter {
public:
    /**
     * @brief 根据传入的食物数据，计算所有食物类别的等效分量。
     * @param totalKcal 输入的总热量值（单位：千卡）。
     * @param allFoodData 一个包含所有食物类别和食物项的向量，通常从外部文件加载。
     * @return 一个map，其中键是食物类别，值是该类别下所有食物的等效分量列表。
     */
    std::map<std::string, std::vector<FoodEquivalent>> calculate(
        double totalKcal,
        const std::vector<FoodCategoryData>& allFoodData
    );

private:
    /**
     * @brief 处理单个食物类别，计算该类别下所有食物的等效分量。
     * @param totalKcal 总热量值。
     * @param categoryName 食物类别的名称。
     * @param foods 包含该类别食物原始数据的向量。
     * @param allEquivalents 用于存储所有计算结果的map的引用。
     */
    void processCategory(
        double totalKcal,
        const std::string& categoryName,
        const std::vector<FoodData>& foods,
        std::map<std::string, std::vector<FoodEquivalent>>& allEquivalents);

    /**
     * @brief 对一个食物等效列表按分量（amount）进行降序排序。
     * @param equivalents 需要排序的食物等效向量的引用。
     */
    void sortEquivalentsByAmount(std::vector<FoodEquivalent>& equivalents);
};

#endif // FOOD_CONVERTER_H