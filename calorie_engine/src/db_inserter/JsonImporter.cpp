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

    // 2. 将JSON数据映射到两个结构体
    long long timestamp = data.value("unix_timestamp", 0LL);
    if (timestamp == 0) {
        throw std::runtime_error("JSON数据缺少主键 'unix_timestamp'");
    }

    if (!data.contains("raw_data") || !data.contains("processed_data")) {
        throw std::runtime_error("JSON数据缺少 'raw_data' 或 'processed_data' 对象");
    }

    const auto& raw_json = data["raw_data"];
    const auto& processed_json = data["processed_data"];

    // 填充原始数据结构体
    RawActivityData raw_data;
    raw_data.unix_timestamp = timestamp;
    raw_data.start_time = raw_json.value("start_time", "");
    raw_data.sport_type = raw_json.value("sport_type", "");
    raw_data.data_source = raw_json.value("data_source", "");
    raw_data.mileage = raw_json.value("mileage", "0.0");
    raw_data.time = raw_json.value("time", "00:00:00");
    raw_data.bpm = raw_json.value("BPM", "0");
    raw_data.max_bpm = raw_json.value("max_BPM", "0");
    raw_data.active_calories = raw_json.value("active_calories", "0");
    raw_data.total_calories = raw_json.value("total_calories", "0");

    // 填充处理后数据结构体
    ProcessedActivityData processed_data;
    processed_data.unix_timestamp = timestamp;
    processed_data.start_time_24 = processed_json.value("start_time_24", "");
    processed_data.hours = processed_json.value("hours", 0);
    processed_data.minutes = processed_json.value("minutes", 0);
    processed_data.seconds = processed_json.value("seconds", 0);
    processed_data.sport_type_en = processed_json.value("sport_type_en", "");

    // 3. 实例化DatabaseManager并插入数据
    DatabaseManager dbManager(dbPath);
    dbManager.insertActivityRecord(raw_data, processed_data);
}