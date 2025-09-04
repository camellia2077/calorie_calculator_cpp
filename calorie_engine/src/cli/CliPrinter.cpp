#include "CliPrinter.h"
#include <iostream>

void CliPrinter::printMainUsage(const char* progName) {
    std::cerr << "用法: " << progName << " <command> [options]\n\n";
    std::cerr << "一个用于计算运动热量消耗和食物等效的工具。\n\n";
    std::cerr << "可用命令:\n";
    std::cerr << "  calculate    根据运动参数计算热量消耗。\n";
    std::cerr << "  import       从JSON文件导入运动记录到数据库。\n\n";
    std::cerr << "全局选项:\n";
    std::cerr << "  -h, --help      显示此帮助信息。\n";
    std::cerr << "  -v, --version   显示程序版本。\n\n";
    std::cerr << "使用 \"" << progName << " <command> --help\" 查看特定命令的帮助。\n";
}

void CliPrinter::printCalculateUsage(const char* progName) {
    std::cerr << "用法: " << progName << " calculate [options]\n\n";
    std::cerr << "根据运动参数计算热量消耗。\n\n";
    std::cerr << "选项:\n";
    std::cerr << "  -s, --sport <type>      [必需] 运动类型 ('run'/'r' 或 'bike'/'b')。\n";
    std::cerr << "  -d, --distance <km>     [必需] 公里数 (例如, 10.5)。\n";
    std::cerr << "  -w, --weight <kg>       [必需] 体重公斤数 (例如, 70.0)。\n";
    std::cerr << "  -H, --hours <num>       [可选] 持续时间的小时部分 (默认为0)。\n";
    std::cerr << "  -M, --minutes <num>     [可选] 持续时间的分钟部分 (默认为0)。\n";
    std::cerr << "  -S, --seconds <num>     [可选] 持续时间的秒数部分 (默认为0)。\n";
    std::cerr << "  -h, --help              显示此帮助信息。\n\n";
    std::cerr << "示例:\n\n";
    std::cerr << "  # 跑步10公里，用时45分30秒，体重75.5公斤\n";
    std::cerr << "  " << progName << " calculate --sport run --minutes 45 --seconds 30 --distance 10 --weight 75.5\n\n";
}

void CliPrinter::printImportUsage(const char* progName) {
    std::cerr << "用法: " << progName << " import [options] <json_file_or_directory>\n\n";
    std::cerr << "从一个或多个JSON文件导入运动记录到数据库。\n\n";
    std::cerr << "参数:\n";
    std::cerr << "  <json_file_or_directory>   单个JSON文件或包含多个JSON文件的目录的路径。\n\n";
    std::cerr << "选项:\n";
    std::cerr << "  --dbpath <directory>       存储SQLite数据库文件的目录 (默认为当前目录)。\n";
    std::cerr << "  -h, --help                 显示此帮助信息。\n\n";
    std::cerr << "示例:\n\n";
    std::cerr << "  # 导入单个文件\n";
    std::cerr << "  " << progName << " import ./my_activities/run_01.json\n\n";
    std::cerr << "  # 导入一个目录下的所有JSON文件\n";
    std::cerr << "  " << progName << " import ./all_runs/ --dbpath ./database/\n";
}