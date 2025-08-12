#include "WorkoutCalculator.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include "food_data.h" // --- 新增：引入食物热量数据 ---

// --- A helper function for safe numeric input ---
// This is best kept as a free-standing template function.
template<typename T>
void getNumericInput(T& value, const std::string& prompt) {
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "输入无效，请输入一个合法的数字: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// =================================================================
// WorkoutCalculator Class Member Function Implementations
// =================================================================

// Constructor: Initializes the calculator with a name and data
WorkoutCalculator::WorkoutCalculator(const std::string& name, const std::vector<DataPoint>& data)
    : activityName(name), dataTable(data) {}

// Main public method to run the calculator's workflow
void WorkoutCalculator::run() {
    getUserInput();
    if (calculateResults()) {
        displayResults();
    }
}

// Gets all necessary input from the user
void WorkoutCalculator::getUserInput() {
    getNumericInput(currentWorkout.timeHr, "请输入您的" + activityName + "时间（小时部分）: ");
    getNumericInput(currentWorkout.timeMin, "请输入您的" + activityName + "时间（分钟部分）: ");
    getNumericInput(currentWorkout.timeSec, "请输入您的" + activityName + "时间（秒的部分）: ");
    getNumericInput(currentWorkout.distanceKm, "请输入您的" + activityName + "距离（公里）: ");
    getNumericInput(currentWorkout.weightKg, "请输入您的体重（公斤）: ");
}

// Performs all calculations and validations
bool WorkoutCalculator::calculateResults() {
    auto& data = currentWorkout; // Use an alias for cleaner code

    // Validate the numerical ranges of the input
    if (data.timeHr < 0 || data.timeMin < 0 || data.timeMin >= 60 || data.timeSec < 0 || data.timeSec >= 60 || data.distanceKm <= 0 || data.weightKg <= 0) {
        std::cerr << "错误：输入值不合法。时间不能为负，分钟和秒数需小于60，距离和体重必须为正数。" << std::endl;
        return false;
    }

    // Perform calculations and store results in the 'currentWorkout' member
    data.totalTimeInMinutes = (data.timeHr * 60.0) + data.timeMin + (data.timeSec / 60.0);
    data.userSpeedKmh = (data.totalTimeInMinutes > 0) ? (data.distanceKm / (data.totalTimeInMinutes / 60.0)) : 0;
    
    // Check if the calculated speed is within the range of our data table
    if (data.userSpeedKmh < dataTable.front().speedKph || data.userSpeedKmh > dataTable.back().speedKph) {
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "您的平均速度是: " << data.userSpeedKmh << " 公里/小时 (km/h)" << std::endl;
        std::cout << "\n错误：您的速度不在有效计算范围内。" << std::endl;
        std::cout << "本程序可计算的速度范围为 " << dataTable.front().speedKph << " 到 " << dataTable.back().speedKph << " 公里/小时。" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return false;
    }

    data.speedMps = data.userSpeedKmh / 3.6;
    double paceDecimal = (data.userSpeedKmh > 0) ? (60.0 / data.userSpeedKmh) : 0;
    data.paceMinutes = static_cast<int>(paceDecimal);
    data.paceSeconds = static_cast<int>(round((paceDecimal - data.paceMinutes) * 60.0));
    
    data.averageMets = getInterpolatedValue(data.userSpeedKmh);
    data.totalKcal = data.averageMets * data.weightKg * (data.totalTimeInMinutes / 60.0);
    const double KJ_PER_KCAL = 4.184;
    data.totalKj = data.totalKcal * KJ_PER_KCAL;
    
    double equivalentRestingMinutes = data.averageMets * data.totalTimeInMinutes;
    data.equivalentHours = static_cast<int>(equivalentRestingMinutes / 60);
    data.equivalentMinutes = static_cast<int>(round(fmod(equivalentRestingMinutes, 60.0)));
    
    return true;
}

// Displays the final formatted report to the user
void WorkoutCalculator::displayResults() {
    const auto& data = currentWorkout; // Use a const alias for read-only access
    const int labelWidth = 28; 

    // 设置输出格式为固定的，并保留一位小数
    std::cout << std::fixed << std::setprecision(1);

    std::cout << "========================================" << std::endl;

    std::cout << "\n[ 运动表现 ]" << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 平均速度 (km/h): " << data.userSpeedKmh << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 平均速度 (m/s): " << data.speedMps << std::endl;
    // 重置 setw，因为它会影响后续输出
    std::cout << std::left << std::setw(0) << "  - 平均配速: " << data.paceMinutes << "分" << data.paceSeconds << "秒 / 公里" << std::endl;

    std::cout << "\n[ 热量消耗 ]" << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 平均代谢当量 (METs): " << data.averageMets << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 总消耗 (千卡/大卡): " << data.totalKcal << std::endl;
    std::cout << std::left << std::setw(labelWidth) << "  - 总消耗 (千焦): " << data.totalKj << std::endl;

    // --- 新增：食物等效换算 ---
    std::cout << "\n[ 食物等效 ]" << std::endl;
    std::cout << "  本次消耗的热量，大约相当于：" << std::endl;
    for (const auto& food : foodCalorieData) {
        if (food.kcalPer100g > 0) {
            double equivalentGrams = data.totalKcal / (food.kcalPer100g / 100.0);
            std::cout << "  - " << equivalentGrams << " 克" << food.name << std::endl;
        }
    }
    // --- 新增结束 ---

    std::cout << "\n[ 等效活动 ]" << std::endl;
    std::cout << "  本次" << activityName << "从热量消耗来看，" << std::endl;
    std::cout << "  相当于静坐了：" << data.equivalentHours << " 小时 " << data.equivalentMinutes << " 分钟" << std::endl;

    std::cout << "========================================" << std::endl;
}

// Interpolates the MET value based on user speed
double WorkoutCalculator::getInterpolatedValue(double userSpeed) {
    if (userSpeed >= dataTable.back().speedKph) return dataTable.back().metValue;
    if (userSpeed <= dataTable.front().speedKph) return dataTable.front().metValue;
    
    for (size_t i = 0; i < dataTable.size() - 1; ++i) {
        if (userSpeed >= dataTable[i].speedKph && userSpeed < dataTable[i + 1].speedKph) {
            double x1 = dataTable[i].speedKph;
            double y1 = dataTable[i].metValue;
            double x2 = dataTable[i+1].speedKph;
            double y2 = dataTable[i+1].metValue;
            return y1 + (userSpeed - x1) * (y2 - y1) / (x2 - x1);
        }
    }
    return 0.0; // Should not be reached if speed is within bounds
}