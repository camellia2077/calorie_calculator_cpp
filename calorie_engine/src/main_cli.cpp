#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "app/Application.h"
#include "common/version.h"
// 确保包含了DbInserterFacade.h，即使这里不直接调用
// 因为Application.h会用到它
#include "db_inserter/DbInserterFacade.h"


#ifdef _WIN32
#include <windows.h>
#endif

// 更新了使用说明
void printUsage(const char* progName) {
    std::cerr << "用法: " << progName << " <sport> <hours> <minutes> <seconds> <distance_km> <weight_kg>" << std::endl;
    std::cerr << "  或: " << progName << " --import <json_file> [--db <database_file>]" << std::endl;
    std::cerr << "  <sport>        : 'run' 或 'r' 代表跑步, 'bike' 或 'b' 代表骑行。" << std::endl;
    std::cerr << "  <hours>        : 持续时间的小时部分 (例如, 1)。" << std::endl;
    std::cerr << "  <minutes>      : 持续时间的分钟部分 (例如, 30)。" << std::endl;
    std::cerr << "  <seconds>      : 持续时间的秒数部分 (例如, 0)。" << std::endl;
    std::cerr << "  <distance_km>  : 公里数 (例如, 10.5)。" << std::endl;
    std::cerr << "  <weight_kg>    : 体重公斤数 (例如, 70.0)。" << std::endl;
    std::cerr << "\n选项:" << std::endl;
    std::cerr << "  --help, -h     : 显示此帮助信息。" << std::endl;
    std::cerr << "  --version, -v  : 显示版本信息。" << std::endl;
    std::cerr << "  --import       : 从一个JSON文件导入单条活动记录。" << std::endl;
    std::cerr << "  --db           : (可选) 指定导入用的SQLite数据库文件路径。" << std::endl;
    std::cerr << "                 :  如果未提供，则默认为 'activities.db'。" << std::endl;
    std::cerr << "\n示例 (计算): " << progName << " r 0 45 30 10.0 75.5" << std::endl;
    std::cerr << "示例 (导入): " << progName << " --import activity.json" << std::endl;
    std::cerr << "示例 (导入到指定DB): " << progName << " --import activity.json --db my_runs.db" << std::endl;
}

int main(int argc, char* argv[]) {
    // 环境设置
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::vector<std::string> args(argv + 1, argv + argc);

    Application app("food_data.json", "output_config.json");

    // --- 修改后的参数解析逻辑 ---
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--import") {
            // 检查 --import 后面是否跟了JSON文件名
            if (i + 1 >= args.size()) {
                std::cerr << "错误: --import 命令需要一个JSON文件路径。" << std::endl;
                printUsage(argv[0]);
                return 1;
            }
            std::string jsonPath = args[i + 1];
            std::string dbPath = "activities.db"; // 默认数据库名称

            // 检查是否提供了 --db 参数来覆盖默认值
            if (i + 2 < args.size() && args[i + 2] == "--db") {
                if (i + 3 < args.size()) {
                    dbPath = args[i + 3];
                } else {
                    std::cerr << "错误: --db 参数需要一个数据库文件路径。" << std::endl;
                    printUsage(argv[0]);
                    return 1;
                }
            }

            app.importActivityFromJson(jsonPath, dbPath);
            return 0; // 导入后退出
        }
    }

    // --- 原有的计算逻辑保持不变 ---
    if (args.empty() || args[0] == "--help" || args[0] == "-h") {
        printUsage(argv[0]);
        return 0;
    }
    if (args[0] == "--version" || args[0] == "-v") {
        std::cout << "Version: " << AppVersion::getVersionString() << std::endl;
        std::cout << "Last Updated: " << AppVersion::LAST_UPDATED << std::endl;
        return 0;
    }

    if (argc != 7) {
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        std::string sportTypeInput = argv[1];
        std::string sportType;
        if (sportTypeInput == "run" || sportTypeInput == "r") {
            sportType = "run";
        } else if (sportTypeInput == "bike" || sportTypeInput == "b") {
            sportType = "bike";
        } else {
            std::cerr << "错误: 无效的运动类型 '" << sportTypeInput << "'." << std::endl;
            return 1;
        }

        WorkoutParameters params;
        params.timeHr = std::stod(argv[2]);
        params.timeMin = std::stod(argv[3]);
        params.timeSec = std::stod(argv[4]);
        params.distanceKm = std::stod(argv[5]);
        params.weightKg = std::stod(argv[6]);
        
        app.runOnce(sportType, params);

    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
