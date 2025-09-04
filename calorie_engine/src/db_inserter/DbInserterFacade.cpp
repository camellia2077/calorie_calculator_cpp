#include "DbInserterFacade.h"
#include "JsonImporter.h" // 包含内部实现
#include <stdexcept>

void DbInserterFacade::importFromJson(const std::string& jsonFilePath, const std::string& dbPath) {
    try {
        // 将调用委托给子系统中的相应组件
        JsonImporter::import(jsonFilePath, dbPath);
    } catch (const std::exception& e) {
        // 可以在这里进行额外的错误记录或处理，
        // 然后重新抛出异常，让调用者知道操作失败了。
        throw std::runtime_error("数据库导入操作失败: " + std::string(e.what()));
    }
}