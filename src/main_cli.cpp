#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// 只需包含 Application.h，它会处理所有核心业务逻辑
#include "app/Application.h"

// 平台设置
#ifdef _WIN32
#include <windows.h>
#endif

// 打印使用说明
void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " <sport> <hours> <minutes> <seconds> <distance_km> <weight_kg>" << std::endl;
    std::cerr << "  <sport>        : 'run' for running, 'bike' for cycling." << std::endl;
    std::cerr << "  <hours>        : Duration hours (e.g., 1)." << std::endl;
    std::cerr << "  <minutes>      : Duration minutes (e.g., 30)." << std::endl;
    std::cerr << "  <seconds>      : Duration seconds (e.g., 0)." << std::endl;
    std::cerr << "  <distance_km>  : Distance in kilometers (e.g., 10.5)." << std::endl;
    std::cerr << "  <weight_kg>    : Your weight in kilograms (e.g., 70.0)." << std::endl;
    std::cerr << "\nExample: " << progName << " run 0 45 30 10.0 75.5" << std::endl;
}

int main(int argc, char* argv[]) {
    // 环境设置
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 1. 检查命令行参数数量是否正确
    if (argc != 7) {
        printUsage(argv[0]);
        return 1; // 返回错误码
    }

    try {
        // 2. 解析命令行参数
        std::string sportType = argv[1];
        WorkoutParameters params;
        params.timeHr = std::stod(argv[2]);
        params.timeMin = std::stod(argv[3]);
        params.timeSec = std::stod(argv[4]);
        params.distanceKm = std::stod(argv[5]);
        params.weightKg = std::stod(argv[6]);

        // 验证运动类型
        if (sportType != "run" && sportType != "bike") {
            std::cerr << "Error: Invalid sport type '" << sportType << "'. Please use 'run' or 'bike'." << std::endl;
            return 1;
        }

        // 3. 创建 Application 实例并调用核心逻辑
        Application app;
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