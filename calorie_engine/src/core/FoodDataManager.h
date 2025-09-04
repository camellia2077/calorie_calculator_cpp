#ifndef FOOD_DATA_MANAGER_H
#define FOOD_DATA_MANAGER_H

#include "data/food_data.h"
#include <vector>
#include <string>

class FoodDataManager {
public:
    // 构造函数：加载并解析JSON文件
    FoodDataManager(const std::string& jsonFilePath);

    // 获取加载后的食物数据
    const std::vector<FoodCategoryData>& getFoodData() const;

private:
    std::vector<FoodCategoryData> allFoodData_;
    
    // 私有辅助函数
    void loadAndParseJson(const std::string& filePath);
    FoodType stringToFoodType(const std::string& str);
};

#endif // FOOD_DATA_MANAGER_H