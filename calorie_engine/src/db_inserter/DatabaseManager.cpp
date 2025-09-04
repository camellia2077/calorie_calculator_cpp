#include "DatabaseManager.h"
#include <stdexcept>
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db_)) {
        throw std::runtime_error("无法打开或创建数据库: " + std::string(sqlite3_errmsg(db_)));
    }
    initializeDatabase();
}

DatabaseManager::~DatabaseManager() {
    if (db_) {
        sqlite3_close(db_);
    }
}

void DatabaseManager::initializeDatabase() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS activity_records (
            start_time_24     TEXT PRIMARY KEY,
            start_time        TEXT,
            sport_type        TEXT,
            data_source       TEXT,
            mileage           REAL,
            time              TEXT,
            BPM               INTEGER,
            active_calories   INTEGER,
            total_calories    INTEGER,
            max_BPM           INTEGER
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::string error = "SQL建表错误: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}

void DatabaseManager::insertActivity(const ActivityData& activity) {
    const char* sql = R"(
        INSERT OR REPLACE INTO activity_records (
            start_time_24, start_time, sport_type, data_source, mileage,
            time, BPM, active_calories, total_calories, max_BPM
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL准备语句失败: " + std::string(sqlite3_errmsg(db_)));
    }

    // 绑定参数
    sqlite3_bind_text(stmt, 1, activity.start_time_24.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, activity.start_time.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, activity.sport_type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, activity.data_source.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 5, activity.mileage);
    sqlite3_bind_text(stmt, 6, activity.time.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, activity.bpm);
    sqlite3_bind_int(stmt, 8, activity.active_calories);
    sqlite3_bind_int(stmt, 9, activity.total_calories);
    sqlite3_bind_int(stmt, 10, activity.max_bpm);

    // 执行
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error = "插入数据失败: " + std::string(sqlite3_errmsg(db_));
        sqlite3_finalize(stmt);
        throw std::runtime_error(error);
    }

    sqlite3_finalize(stmt);
    std::cout << "成功插入记录: " << activity.start_time_24 << std::endl;
}