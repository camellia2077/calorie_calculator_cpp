#include "FoodDataManager.h"
#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp" // 引入nlohmann/json库

// 使用json别名
using json = nlohmann::json;

FoodDataManager::FoodDataManager(const std::string& jsonFilePath) {
    loadAndParseJson(jsonFilePath);
}

const std::vector<FoodCategoryData>& FoodDataManager::getFoodData() const {
    return allFoodData_;
}

void FoodDataManager::loadAndParseJson(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开食物数据文件: " + filePath);
    }

    json data = json::parse(file);

    for (const auto& category : data["food_categories"]) {
        FoodCategoryData currentCategory;
        currentCategory.categoryName = category["category_name"];

        for (const auto& item : category["items"]) {
            FoodData currentFood;
            currentFood.name = item["name"];
            currentFood.kcalPer100 = item["kcalPer100"];
            currentFood.type = stringToFoodType(item["type"]);
            currentCategory.items.push_back(currentFood);
        }
        allFoodData_.push_back(currentCategory);
    }
}

FoodType FoodDataManager::stringToFoodType(const std::string& str) {
    if (str == "Solid") return FoodType::Solid;
    if (str == "Liquid") return FoodType::Liquid;
    return FoodType::Unknown; // 默认或错误情况
}