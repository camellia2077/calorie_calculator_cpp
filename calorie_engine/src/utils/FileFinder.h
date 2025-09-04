#ifndef FILE_FINDER_H
#define FILE_FINDER_H

#include <string>
#include <vector>

/**
 * @class FileFinder
 * @brief 一个用于在文件系统中按后缀名查找文件的通用工具类。
 */
class FileFinder {
public:
    /**
     * @brief 根据给定路径和文件后缀查找一个或多个文件。
     *
     * - 如果路径指向一个匹配后缀的文件，则返回包含该文件路径的列表。
     * - 如果路径指向一个目录，则递归地查找该目录下所有匹配后缀的文件。
     *
     * @param inputPath 要搜索的文件或目录路径。
     * @param extension 要查找的文件后缀 (例如, ".json", ".txt")。
     * @return std::vector<std::string> 包含所有找到的文件完整路径的列表。
     */
    static std::vector<std::string> find(const std::string& inputPath, const std::string& extension);
};

#endif // FILE_FINDER_H
