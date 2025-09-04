#ifndef JSON_IMPORTER_H
#define JSON_IMPORTER_H

#include <string>

class JsonImporter {
public:
    /**
     * @brief 从指定的JSON文件导入运动数据到SQLite数据库。
     * @param jsonFilePath JSON文件的路径。
     * @param dbPath 目标SQLite数据库文件的路径。
     */
    static void import(const std::string& jsonFilePath, const std::string& dbPath);
};

#endif // JSON_IMPORTER_H