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
    char* errMsg = nullptr;
    const char* create_raw_sql = R"(
        CREATE TABLE IF NOT EXISTS raw_records (
            unix_timestamp    INTEGER PRIMARY KEY,
            start_time        TEXT,
            sport_type        TEXT,
            data_source       TEXT,
            mileage           TEXT,
            time              TEXT,
            BPM               TEXT,
            max_BPM           TEXT,
            active_calories   TEXT,
            total_calories    TEXT
        );
    )";
    if (sqlite3_exec(db_, create_raw_sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::string error = "SQL创建raw_records表错误: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }

    const char* create_processed_sql = R"(
        CREATE TABLE IF NOT EXISTS processed_records (
            unix_timestamp    INTEGER PRIMARY KEY,
            start_time_24     TEXT,
            hours             INTEGER,
            minutes           INTEGER,
            seconds           INTEGER,
            sport_type_en     TEXT
        );
    )";
    if (sqlite3_exec(db_, create_processed_sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::string error = "SQL创建processed_records表错误: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}

void DatabaseManager::insertActivityRecord(const RawActivityData& rawData, const ProcessedActivityData& processedData) {
    sqlite3_exec(db_, "BEGIN TRANSACTION;", 0, 0, 0); // 开始事务

    sqlite3_stmt* raw_stmt = nullptr;
    sqlite3_stmt* processed_stmt = nullptr;

    try {
        // 1. 准备并执行对 raw_records 的插入
        const char* raw_sql = R"(
            INSERT OR REPLACE INTO raw_records (
                unix_timestamp, start_time, sport_type, data_source, mileage,
                time, BPM, max_BPM, active_calories, total_calories
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
        )";
        if (sqlite3_prepare_v2(db_, raw_sql, -1, &raw_stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("准备raw_records插入语句失败: " + std::string(sqlite3_errmsg(db_)));
        }
        sqlite3_bind_int64(raw_stmt, 1, rawData.unix_timestamp);
        sqlite3_bind_text(raw_stmt, 2, rawData.start_time.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(raw_stmt, 3, rawData.sport_type.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(raw_stmt, 4, rawData.data_source.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(raw_stmt, 5, rawData.mileage.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(raw_stmt, 6, rawData.time.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(raw_stmt, 7, rawData.bpm.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(raw_stmt, 8, rawData.max_bpm.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(raw_stmt, 9, rawData.active_calories.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(raw_stmt, 10, rawData.total_calories.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(raw_stmt) != SQLITE_DONE) {
            throw std::runtime_error("插入raw_records数据失败: " + std::string(sqlite3_errmsg(db_)));
        }
        sqlite3_finalize(raw_stmt);
        raw_stmt = nullptr;

        // 2. 准备并执行对 processed_records 的插入
        const char* processed_sql = R"(
            INSERT OR REPLACE INTO processed_records (
                unix_timestamp, start_time_24, hours, minutes, seconds, sport_type_en
            ) VALUES (?, ?, ?, ?, ?, ?);
        )";
        if (sqlite3_prepare_v2(db_, processed_sql, -1, &processed_stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("准备processed_records插入语句失败: " + std::string(sqlite3_errmsg(db_)));
        }
        sqlite3_bind_int64(processed_stmt, 1, processedData.unix_timestamp);
        sqlite3_bind_text(processed_stmt, 2, processedData.start_time_24.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(processed_stmt, 3, processedData.hours);
        sqlite3_bind_int(processed_stmt, 4, processedData.minutes);
        sqlite3_bind_int(processed_stmt, 5, processedData.seconds);
        sqlite3_bind_text(processed_stmt, 6, processedData.sport_type_en.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(processed_stmt) != SQLITE_DONE) {
            throw std::runtime_error("插入processed_records数据失败: " + std::string(sqlite3_errmsg(db_)));
        }
        sqlite3_finalize(processed_stmt);
        processed_stmt = nullptr;

        sqlite3_exec(db_, "COMMIT;", 0, 0, 0); // 提交事务
        std::cout << "成功插入记录 (Unix Timestamp): " << rawData.unix_timestamp << std::endl;

    } catch (...) {
        if (raw_stmt) sqlite3_finalize(raw_stmt);
        if (processed_stmt) sqlite3_finalize(processed_stmt);
        sqlite3_exec(db_, "ROLLBACK;", 0, 0, 0); // 回滚事务
        throw; // 重新抛出异常
    }
}