#ifndef DB_INSERTER_FACADE_H
#define DB_INSERTER_FACADE_H

#include <string>

/**
 * @class DbInserterFacade
 * @brief 为数据库导入功能提供一个简单的统一接口。
 *
 * 这是客户端与 db_inserter 子系统交互的唯一入口点，
 * 隐藏了内部关于JSON解析和数据库管理的实现细节。
 */
class DbInserterFacade {
public:
    /**
     * @brief 从一个JSON文件导入一条活动记录到指定的SQLite数据库。
     * @param jsonFilePath 源JSON文件的路径。
     * @param dbPath 目标SQLite数据库文件的路径。
     * @throw std::runtime_error 如果文件读取、JSON解析或数据库操作失败。
     */
    static void importFromJson(const std::string& jsonFilePath, const std::string& dbPath);
};

#endif // DB_INSERTER_FACADE_H