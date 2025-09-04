#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <sqlite3.h>

// 定义一个结构体来封装从JSON中解析出的单条运动数据
struct ActivityData {
    std::string start_time_24;
    std::string start_time; // FIX: Was std.string, changed to std::string
    std::string sport_type;
    std::string data_source;
    double mileage;
    std::string time;
    int bpm;
    int active_calories;
    int total_calories;
    int max_bpm;
};

class DatabaseManager {
public:
    // 构造函数，传入数据库文件路径并打开连接
    DatabaseManager(const std::string& dbPath);
    // 析构函数，关闭数据库连接
    ~DatabaseManager();
    // 插入一条运动记录
    void insertActivity(const ActivityData& activity);

private:
    sqlite3* db_ = nullptr;
    // 初始化数据库，如果表不存在则创建它
    void initializeDatabase();
};

#endif // DATABASE_MANAGER_H
