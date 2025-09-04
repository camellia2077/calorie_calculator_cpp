#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>

#include "app/Application.h"
#include "common/version.h"
#include "db_inserter/DbInserterFacade.h"

#ifdef _WIN32
#include <windows.h>
#endif

// --- 帮助信息函数 ---
void printMainUsage(const char* progName);
void printCalculateUsage(const char* progName);
void printImportUsage(const char* progName);

// --- 子命令处理函数 ---
void handleCalculate(const std::vector<std::string>& args, Application& app);
void handleImport(const std::vector<std::string>& args, Application& app);


int main(int argc, char* argv[]) {
    // 环境设置
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        printMainUsage(argv[0]);
        return 0;
    }

    if (args[0] == "-v" || args[0] == "--version") {
        std::cout << "Version: " << AppVersion::getVersionString() << std::endl;
        std::cout << "Last Updated: " << AppVersion::LAST_UPDATED << std::endl;
        return 0;
    }

    Application app("food_data.json", "output_config.json");
    std::string command = args[0];
    
    // 移除第一个参数（命令本身），方便后续处理
    std::vector<std::string> command_args(args.begin() + 1, args.end());

    try {
        if (command == "calculate") {
            handleCalculate(command_args, app);
        } else if (command == "import") {
            handleImport(command_args, app);
        } else {
            std::cerr << "错误: 未知命令 '" << command << "'" << std::endl;
            printMainUsage(argv[0]);
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "操作失败: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


// --- 函数实现 ---

void handleCalculate(const std::vector<std::string>& args, Application& app) {
    WorkoutParameters params = {0, 0, 0, 0, 0};
    std::string sportType;

    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];
        if ((arg == "-s" || arg == "--sport") && i + 1 < args.size()) {
            sportType = args[++i];
        } else if ((arg == "-H" || arg == "--hours") && i + 1 < args.size()) {
            params.timeHr = std::stod(args[++i]);
        } else if ((arg == "-M" || arg == "--minutes") && i + 1 < args.size()) {
            params.timeMin = std::stod(args[++i]);
        } else if ((arg == "-S" || arg == "--seconds") && i + 1 < args.size()) {
            params.timeSec = std::stod(args[++i]);
        } else if ((arg == "-d" || arg == "--distance") && i + 1 < args.size()) {
            params.distanceKm = std::stod(args[++i]);
        } else if ((arg == "-w" || arg == "--weight") && i + 1 < args.size()) {
            params.weightKg = std::stod(args[++i]);
        } else if (arg == "--help" || arg == "-h") {
            printCalculateUsage("program"); // progName is not easily available here
            return;
        }
    }

    if (sportType.empty() || params.distanceKm <= 0 || params.weightKg <= 0) {
        throw std::runtime_error("错误: 'sport', 'distance' 和 'weight' 是必须提供的参数。");
    }
    
    std::string sport;
    if (sportType == "run" || sportType == "r") sport = "run";
    else if (sportType == "bike" || sportType == "b") sport = "bike";
    else throw std::runtime_error("错误: 无效的运动类型 '" + sportType + "'");

    app.runOnce(sport, params);
}

void handleImport(const std::vector<std::string>& args, Application& app) {
    std::string jsonPath;
    std::string dbDirectory = ".";

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--dbpath" && i + 1 < args.size()) {
            dbDirectory = args[++i];
        } else if (args[i] == "--help" || args[i] == "-h") {
            printImportUsage("program");
            return;
        } else {
            // 假设不是选项的就是文件路径
            if (jsonPath.empty()) {
                jsonPath = args[i];
            } else {
                throw std::runtime_error("错误: 指定了多个JSON文件路径。");
            }
        }
    }

    if (jsonPath.empty()) {
        throw std::runtime_error("错误: 未指定要导入的JSON文件或目录。");
    }
    
    std::filesystem::path finalDbPath(dbDirectory);
    finalDbPath /= "activities.sqlite3";

    app.importActivityFromJson(jsonPath, finalDbPath.string());
}

void printMainUsage(const char* progName) {
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

void printCalculateUsage(const char* progName) {
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
    std::cerr << "  # 示例 1: 跑步10公里，用时45分30秒，体重75.5公斤\n";
    std::cerr << "  " << progName << " calculate --sport run --minutes 45 --seconds 30 --distance 10 --weight 75.5\n\n";
    std::cerr << "  # 示例 2: 使用短选项进行一次半程马拉松计算 (21.1公里)\n";
    std::cerr << "  " << progName << " calculate -s r -H 1 -M 55 -d 21.1 -w 70\n\n";
    std::cerr << "  # 示例 3: 骑行25公里，用时1小时，体重80公斤\n";
    std::cerr << "  " << progName << " calculate -s bike -H 1 -d 25 -w 80\n\n";
    std::cerr << "  # 示例 4: 一个简短的5公里跑步，只提供分钟\n";
    std::cerr << "  " << progName << " calculate -s run -M 30 -d 5 -w 65\n";
}

void printImportUsage(const char* progName) {
    std::cerr << "用法: " << progName << " import [options] <json_file_or_directory>\n\n";
    std::cerr << "从一个或多个JSON文件导入运动记录到数据库。\n\n";
    std::cerr << "参数:\n";
    std::cerr << "  <json_file_or_directory>   单个JSON文件或包含多个JSON文件的目录的路径。\n\n";
    std::cerr << "选项:\n";
    std::cerr << "  --dbpath <directory>       存储SQLite数据库文件的目录 (默认为当前目录)。\n";
    std::cerr << "  -h, --help                 显示此帮助信息。\n\n";
    std::cerr << "示例:\n\n";
    std::cerr << "  # 导入单个文件到当前目录的数据库\n";
    std::cerr << "  " << progName << " import ./my_activities/run_01.json\n\n";
    std::cerr << "  # 导入一个目录下的所有JSON文件，并指定数据库位置\n";
    std::cerr << "  " << progName << " import ./all_runs/ --dbpath ./database/\n";
}