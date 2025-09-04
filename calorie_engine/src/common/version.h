#ifndef VERSION_H
#define VERSION_H

#include <string>

// 该文件集中管理应用程序的版本信息

namespace AppVersion {

// 主版本号：对应重大的功能更新或架构变化
constexpr int MAJOR_VERSION = 0;

// 次版本号：对应新增功能或较大范围的重构
constexpr int MINOR_VERSION = 2;

// 修订号：对应小规模的错误修复或优化
constexpr int PATCH_VERSION = 0;

// 最后更新日期
const std::string LAST_UPDATED = "2025-09-04";

// 将版本号格式化为字符串，例如 "1.0.0"
inline std::string getVersionString() {
    return std::to_string(MAJOR_VERSION) + "." +
           std::to_string(MINOR_VERSION) + "." +
           std::to_string(PATCH_VERSION);
}

} // namespace AppVersion

#endif // VERSION_H