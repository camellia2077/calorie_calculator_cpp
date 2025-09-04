#include "JsonImporter.h"
#include "DatabaseManager.h"
#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void JsonImporter::import(const std::string& jsonFilePath, const std::string& dbPath) {
    // 1. 打开并读取JSON文件
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开JSON文件: " + jsonFilePath);
    }

    json data;
    try {
        data = json::parse(file);
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON解析失败: " + std::string(e.what()));
    }

    // 2. 将JSON数据映射到ActivityData结构体
    ActivityData activity;
    activity.start_time_24 = data.value("start_time_24", "");
    activity.start_time = data.value("start_time", "");
    activity.sport_type = data.value("sport_type", "");
    activity.data_source = data.value("data_source", "");
    activity.time = data.value("time", "00:00:00");
    // 对可能为字符串的数字进行转换
    activity.mileage = std::stod(data.value("mileage", "0.0"));
    activity.bpm = std::stoi(data.value("BPM", "0"));
    activity.active_calories = std::stoi(data.value("active_calories", "0"));
    activity.total_calories = std::stoi(data.value("total_calories", "0"));
    activity.max_bpm = std::stoi(data.value("max_BPM", "0"));

    // 主键不能为空
    if (activity.start_time_24.empty()) {
        throw std::runtime_error("JSON数据缺少主键 'start_time_24'");
    }

    // 3. 实例化DatabaseManager并插入数据
    DatabaseManager dbManager(dbPath);
    dbManager.insertActivity(activity);
}