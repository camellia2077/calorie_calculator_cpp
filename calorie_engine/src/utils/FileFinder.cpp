#include "FileFinder.h"
#include <filesystem> // C++17 标准库，用于文件系统操作

namespace fs = std::filesystem;

std::vector<std::string> FileFinder::find(const std::string& inputPath, const std::string& extension) {
    std::vector<std::string> foundFiles;

    if (extension.empty() || extension[0] != '.') {
        // 为确保比较正确，可以增加一个错误检查或自动添加点
        // 这里我们假设调用者会提供正确的格式，如 ".json"
    }

    // 检查路径是否存在
    if (!fs::exists(inputPath)) {
        return foundFiles; // 路径不存在，返回空列表
    }

    // 情况1：路径是一个常规文件
    if (fs::is_regular_file(inputPath)) {
        if (fs::path(inputPath).extension() == extension) {
            foundFiles.push_back(inputPath);
        }
    }
    // 情况2：路径是一个目录
    else if (fs::is_directory(inputPath)) {
        // 使用递归目录迭代器遍历所有文件
        for (const auto& entry : fs::recursive_directory_iterator(inputPath)) {
            if (entry.is_regular_file() && entry.path().extension() == extension) {
                foundFiles.push_back(entry.path().string());
            }
        }
    }

    return foundFiles;
}
