#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <sqlite3.h>

// 结构体，用于封装来自 JSON "raw_data" 对象的数据
struct RawActivityData {
    long long unix_timestamp; // 主键
    std::string start_time;
    std::string sport_type;
    std::string data_source;
    std::string mileage;
    std::string time;
    std::string bpm;
    std::string active_calories;
    std::string total_calories;
    std::string max_bpm;
};

// 结构体，用于封装来自 JSON "processed_data" 对象的数据
struct ProcessedActivityData {
    long long unix_timestamp; // 主键和外键
    std::string start_time_24;
    int hours;
    int minutes;
    int seconds;
    std::string sport_type_en;
};

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    // 新方法：同时插入原始记录和处理后的记录
    void insertActivityRecord(const RawActivityData& rawData, const ProcessedActivityData& processedData);

private:
    sqlite3* db_ = nullptr;
    void initializeDatabase();
};

#endif // DATABASE_MANAGER_H