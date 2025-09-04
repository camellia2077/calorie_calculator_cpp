#include "cli/CliHandler.h"
#include "app/Application.h"
#include "common/version.h"
#include "cli/CliPrinter.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>

CliHandler::CliHandler(Application& app) : app_(app) {}

CliAction CliHandler::run(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        return CliAction::PRINT_MAIN_HELP;
    }

    if (args[0] == "-v" || args[0] == "--version") {
        std::cout << "Version: " << AppVersion::getVersionString() << std::endl;
        std::cout << "Last Updated: " << AppVersion::LAST_UPDATED << std::endl;
        return CliAction::SUCCESS;
    }

    std::string command = args[0];
    std::vector<std::string> command_args(args.begin() + 1, args.end());

    try {
        if (command == "calculate") {
            return handleCalculate(command_args);
        } else if (command == "import") {
            return handleImport(command_args);
        } else {
            std::cerr << "错误: 未知命令 '" << command << "'" << std::endl;
            return CliAction::PRINT_MAIN_HELP;
        }
    } catch (const std::exception& e) {
        std::cerr << "操作失败: " << e.what() << std::endl;
        return CliAction::COMMAND_ERROR; // <-- Changed
    }
}

CliAction CliHandler::handleCalculate(const std::vector<std::string>& args) {
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
            return CliAction::PRINT_CALCULATE_HELP;
        }
    }

    if (sportType.empty() || params.distanceKm <= 0 || params.weightKg <= 0) {
        throw std::runtime_error("错误: 'sport', 'distance' 和 'weight' 是必须提供的参数。");
    }
    
    std::string sport;
    if (sportType == "run" || sportType == "r") sport = "run";
    else if (sportType == "bike" || sportType == "b") sport = "bike";
    else throw std::runtime_error("错误: 无效的运动类型 '" + sportType + "'");

    app_.runOnce(sport, params);
    return CliAction::SUCCESS;
}

CliAction CliHandler::handleImport(const std::vector<std::string>& args) {
    std::string jsonPath;
    std::string dbDirectory = ".";

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--dbpath" && i + 1 < args.size()) {
            dbDirectory = args[++i];
        } else if (args[i] == "--help" || args[i] == "-h") {
            return CliAction::PRINT_IMPORT_HELP;
        } else {
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

    app_.importActivityFromJson(jsonPath, finalDbPath.string());
    return CliAction::SUCCESS;
}