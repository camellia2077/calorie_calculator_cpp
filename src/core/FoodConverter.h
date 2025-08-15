#ifndef FOOD_CONVERTER_H
#define FOOD_CONVERTER_H

#include <vector>
#include <map>
#include <string>
#include "domain/FoodEquivalent.h"
#include "domain/WorkoutDomain.h"
#include "data/food_data.h" // 食物名称和热量的对照

/**
 * @class FoodConverter
 * @brief 一个用于将热量值转换为等效食物分量的工具类。
 *
 * 该类接收一个总热量值（例如通过运动消耗的卡ло里），
 * 然后计算出不同种类的常见食物需要多少分量才能达到这个热量值。
 */
class FoodConverter {
public:
    /**
     * @brief 计算并返回所有食物类别的等效分量。
     * @param totalKcal 输入的总热量值（单位：千卡）。
     * @return 一个map，其中键是食物类别（如“常见水果”），值是该类别下所有食物的等效分量列表。
     */
    std::map<std::string, std::vector<FoodEquivalent>> calculate(double totalKcal);

private:
    /**
     * @brief 处理单个食物类别，计算该类别下所有食物的等效分量。
     * @param totalKcal 总热量值。
     * @param categoryName 食物类别的名称。
     * @param foods 包含该类别食物原始数据的向量。
     * @param unit 该类别食物分量的单位（如“克”或“毫升”）。
     * @param allEquivalents 用于存储所有计算结果的map的引用。
     */
    void processCategory(
        double totalKcal,
        const std::string& categoryName,
        const std::vector<FoodData>& foods,
        const std::string& unit,
        std::map<std::string, std::vector<FoodEquivalent>>& allEquivalents
    );

    /**
     * @brief 对一个食物等效列表按分量（amount）进行降序排序。
     * @param equivalents 需要排序的食物等效向量的引用。
     */
    void sortEquivalentsByAmount(std::vector<FoodEquivalent>& equivalents);
};

#endif // FOOD_CONVERTER_H