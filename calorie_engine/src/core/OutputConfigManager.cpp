#include "OutputConfigManager.h"
#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp" // 引入nlohmann/json库

using json = nlohmann::json;

OutputConfigManager::OutputConfigManager(const std::string& jsonFilePath) {
    loadAndParseJson(jsonFilePath);
}

const OutputConfiguration& OutputConfigManager::getOutputConfig() const {
    return config_;
}

void OutputConfigManager::loadAndParseJson(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开输出配置文件: " + filePath);
    }

    json data = json::parse(file);

    const auto& options = data["display_options"];
    config_.showWorkoutPerformance = options.value("show_workout_performance", false);
    config_.showCalorieConsumption = options.value("show_calorie_consumption", false);
    config_.showFoodEquivalents = options.value("show_food_equivalents", false);
    config_.showEquivalentActivity = options.value("show_equivalent_activity", false);
}