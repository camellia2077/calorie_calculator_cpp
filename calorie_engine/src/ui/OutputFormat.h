#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <string>
#include <vector>
#include <map>
#include "domain/WorkoutDomain.h"
#include "domain/FoodEquivalent.h"
#include "core/OutputConfigManager.h" // 新增

class OutputFormat {
public:
    // --- 显示功能 ---
    void displayVersionInfo(const std::string& version, const std::string& lastUpdated);
    void displayGoodbyeMessage();
    // 修改 displayResults 方法签名
    void displayResults(const WorkoutResults& results,
                        const std::string& activityName,
                        const std::map<std::string, std::vector<FoodEquivalent>>& foodEquivalents,
                        const OutputConfiguration& config);
    void displayError(const std::string& message);
    void displaySpeedError(double userSpeed, double minSpeed, double maxSpeed);
};

#endif // CONSOLE_UI_H