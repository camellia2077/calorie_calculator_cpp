#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// 只需包含 Application.h，它会处理所有核心业务逻辑
#include "app/Application.h"
// 新增：包含版本信息头文件，以便访问版本号
#include "common/version.h"

// 平台设置
#ifdef _WIN32
#include <windows.h>
#endif

// 打印使用说明 (已更新)
void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " <sport> <hours> <minutes> <seconds> <distance_km> <weight_kg>" << std::endl;
    std::cerr << "  <sport>        : 'run' or 'r' for running, 'bike' or 'b' for cycling." << std::endl;
    std::cerr << "  <hours>        : Duration hours (e.g., 1)." << std::endl;
    std::cerr << "  <minutes>      : Duration minutes (e.g., 30)." << std::endl;
    std::cerr << "  <seconds>      : Duration seconds (e.g., 0)." << std::endl;
    std::cerr << "  <distance_km>  : Distance in kilometers (e.g., 10.5)." << std::endl;
    std::cerr << "  <weight_kg>    : Your weight in kilograms (e.g., 70.0)." << std::endl;
    std::cerr << "\nOptions:" << std::endl;
    std::cerr << "  --help, -h     : Show this help message." << std::endl;
    std::cerr << "  --version, -v  : Show version information." << std::endl;
    std::cerr << "\nExample: " << progName << " r 0 45 30 10.0 75.5" << std::endl;
}

int main(int argc, char* argv[]) {
    // 环境设置
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 新增：检查帮助或版本号请求
    // 当只有一个参数时，检查它是否为帮助或版本号标志
    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0; // 正常退出
        }
        if (arg == "--version" || arg == "-v") {
            // 从 AppVersion 命名空间获取版本信息
            std::cout << "Version: " << AppVersion::getVersionString() << std::endl;
            std::cout << "Last Updated: " << AppVersion::LAST_UPDATED << std::endl;
            return 0; // 正常退出
        }
    }

    // 1. 检查执行计算所需的参数数量是否正确
    if (argc != 7) {
        printUsage(argv[0]);
        return 1; // 返回错误码
    }

    // 使用新的构造函数实例化 Application 对象，并传入食物数据路径
    Application app("food_data.json");

    try {
        // 2. 解析命令行参数
        std::string sportTypeInput = argv[1];
        std::string sportType;

        // 修改：处理运动类型的别名
        if (sportTypeInput == "run" || sportTypeInput == "r") {
            sportType = "run";
        } else if (sportTypeInput == "bike" || sportTypeInput == "b") {
            sportType = "bike";
        } else {
            std::cerr << "Error: Invalid sport type '" << sportTypeInput << "'. Please use 'run', 'r', 'bike', or 'b'." << std::endl;
            return 1;
        }

        WorkoutParameters params;
        params.timeHr = std::stod(argv[2]);
        params.timeMin = std::stod(argv[3]);
        params.timeSec = std::stod(argv[4]);
        params.distanceKm = std::stod(argv[5]);
        params.weightKg = std::stod(argv[6]);

        // 3. 创建 Application 实例并调用核心逻辑
        app.runOnce(sportType, params);

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid number format in arguments. Please provide valid numbers." << std::endl;
        printUsage(argv[0]);
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: A numeric argument is out of range." << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    return 0; // 成功退出
}