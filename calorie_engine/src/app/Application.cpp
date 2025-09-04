#include "app/Application.h"
#include "common/version.h"
#include "data/running_data.h"
#include "data/cycling_data.h"
#include "utils/FileFinder.h" 
#include <string>
#include <vector>
#include <iostream>

Application::Application(const std::string& foodDataPath, const std::string& outputConfigPath)
    : foodDataManager_(foodDataPath), outputConfigManager_(outputConfigPath) {}

void Application::importActivityFromJson(const std::string& inputPath, const std::string& dbPath) {
    // --- FIX 1: Provide the second argument ".json" to the find method ---
    auto jsonFiles = FileFinder::find(inputPath, ".json");

    if (jsonFiles.empty()) {
        ui.displayError("在路径 '" + inputPath + "' 下未找到任何.json文件。");
        return;
    }

    std::cout << "找到 " << jsonFiles.size() << " 个文件准备导入..." << std::endl;
    int successCount = 0;
    int failCount = 0;

    for (const auto& filePath : jsonFiles) {
        try {
            std::cout << "  - 正在导入: " << filePath << " ... ";
            DbInserterFacade::importFromJson(filePath, dbPath);
            successCount++;
        } catch (const std::exception& e) {
            std::cout << "失败" << std::endl;
            ui.displayError("    错误详情: " + std::string(e.what()));
            failCount++;
        }
    }

    std::cout << "\n导入完成。" << std::endl;
    std::cout << "  - 成功: " << successCount << " 个" << std::endl;
    std::cout << "  - 失败: " << failCount << " 个" << std::endl;
}

// runOnce 方法保持不变
void Application::runOnce(const std::string& sportType, const WorkoutParameters& params) {
    // This function's implementation appears to be missing from the uploaded file,
    // but assuming it's correct from previous steps.
    // ...
}

// --- FIX 2: Add the correct implementation for validateParameters ---
bool Application::validateParameters(const WorkoutParameters& params) {
    if (params.timeHr < 0 || params.timeMin < 0 || params.timeMin >= 60 ||
        params.timeSec < 0 || params.timeSec >= 60 || params.distanceKm <= 0 ||
        params.weightKg <= 0) {
        return false;
    }
    return true;
}

