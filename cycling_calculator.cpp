// 重要提示：请务必将此源文件保存为 UTF-8 编码格式！
#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip> // 用于设置输出精度
#include <cmath>   // 用于 floor, round, fmod 函数

// 为 Windows 平台引入头文件，用于设置控制台编码
#ifdef _WIN32
#include <windows.h>
#endif

// --- 将数据定义为全局常量 ---
// 数据来源: Compendium of Physical Activities (Bicycling)
// 原始单位为 mph (英里/小时)，已转换为 km/h (公里/小时)
const std::vector<double> speedsKph = {
    8.85,   // 5.5 mph
    15.13,  // 9.4 mph
    17.62,  // 10.95 mph (avg of 10-11.9)
    20.84,  // 12.95 mph (avg of 12-13.9)
    24.06,  // 14.95 mph (avg of 14-15.9)
    28.16,  // 17.5 mph (avg of 16-19)
    32.19   // 20 mph
};

const std::vector<double> mets = {
    3.5,
    5.8,
    6.8,
    8.0,
    10.0,
    12.0,
    16.8
};


// 函数声明
double getInterpolatedValue(double userSpeed, const std::vector<double>& x_data, const std::vector<double>& y_data);

int main() {
    // 设置控制台编码以支持中文 (UTF-8)
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    // 设置cout输出小数点后两位的精度
    std::cout << std::fixed << std::setprecision(2);

    // --- 修改：增加了 timeHr 变量 ---
    double timeHr = 0, timeMin = 0, timeSec = 0, distanceKm = 0, weightKg = 0;

    // --- 修改：更新了输入部分 ---
    std::cout << "请输入您的骑行时间（小时部分）: ";
    std::cin >> timeHr;
    std::cout << "请输入您的骑行时间（分钟部分）: ";
    std::cin >> timeMin;
    std::cout << "请输入您的骑行时间（秒的部分）: ";
    std::cin >> timeSec;

    std::cout << "请输入您的骑行距离（公里）: ";
    std::cin >> distanceKm;

    std::cout << "请输入您的体重（公斤）: ";
    std::cin >> weightKg;

    // --- 修改：更新了输入验证 ---
    if (timeHr < 0 || timeMin < 0 || timeMin >= 60 || timeSec < 0 || timeSec >= 60 || distanceKm <= 0 || weightKg <= 0) {
        std::cerr << "错误：输入值不合法。时间不能为负，分钟和秒数需小于60，距离和体重必须为正数。" << std::endl;
        return 1;
    }

    // --- 修改：更新了总分钟数的计算公式 ---
    double totalTimeInMinutes = (timeHr * 60.0) + timeMin + (timeSec / 60.0);
    double userSpeedKmh = (totalTimeInMinutes > 0) ? (distanceKm / (totalTimeInMinutes / 60.0)) : 0;
    
    // 计算其他速度和配速单位
    double speedMps = userSpeedKmh / 3.6;
    double paceDecimal = (userSpeedKmh > 0) ? (60.0 / userSpeedKmh) : 0;
    int paceMinutes = static_cast<int>(paceDecimal);
    int paceSeconds = static_cast<int>(round((paceDecimal - paceMinutes) * 60.0));

    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "您的平均速度是: " << userSpeedKmh << " 公里/小时 (km/h)" << std::endl;
    std::cout << "               " << "(" << speedMps << " 米/秒 (m/s))" << std::endl;
    std::cout << "您的平均配速是: " << paceMinutes << "分" << paceSeconds << "秒 / 公里" << std::endl;
    
    // 速度范围检查
    if (userSpeedKmh < speedsKph.front() || userSpeedKmh > speedsKph.back()) {
        std::cout << "\n错误：您的速度不在有效计算范围内。" << std::endl;
        std::cout << "本程序可计算的速度范围为 " << speedsKph.front() << " 到 " << speedsKph.back() << " 公里/小时。" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return 1;
    }
    
    // ++++++++++ 主要计算逻辑 (无需改动) ++++++++++
    double averageMets = getInterpolatedValue(userSpeedKmh, speedsKph, mets);
    double totalKcal = averageMets * weightKg * (totalTimeInMinutes / 60.0);
    const double KJ_PER_KCAL = 4.184;
    double totalKj = totalKcal * KJ_PER_KCAL;
    double equivalentRestingMinutes = averageMets * totalTimeInMinutes;
    int equivalentHours = static_cast<int>(equivalentRestingMinutes / 60);
    int equivalentMinutesPart = static_cast<int>(round(fmod(equivalentRestingMinutes, 60.0)));

    // --- 结果输出部分 ---
    std::cout << "\n计算结果:" << std::endl;
    std::cout << "本次骑行平均代谢当量 (METs): " << averageMets << std::endl;
    std::cout << "您消耗的总热量约为: " << totalKj << " 千焦 (kJ)" << std::endl;
    std::cout << "                  " << "或 " << totalKcal << " 千卡/大卡 (kcal)" << std::endl;
    std::cout << "\n从消耗热量来看，本次骑行相当于静坐了：" << std::endl;
    std::cout << equivalentHours << " 小时 " << equivalentMinutesPart << " 分钟" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    return 0;
}

/**
 * @brief 根据用户速度，从数据表中插值计算一个Y值（如METs）
 */
double getInterpolatedValue(double userSpeed, const std::vector<double>& x_data, const std::vector<double>& y_data) {
    double interpolatedValue = 0.0;
    
    if (userSpeed >= x_data.back()) {
        interpolatedValue = y_data.back();
    } else if (userSpeed <= x_data.front()) {
        interpolatedValue = y_data.front();
    }
    else {
        for (size_t i = 0; i < x_data.size() - 1; ++i) {
            if (userSpeed >= x_data[i] && userSpeed < x_data[i + 1]) {
                double x1 = x_data[i];
                double y1 = y_data[i];
                double x2 = x_data[i + 1];
                double y2 = y_data[i + 1];
                interpolatedValue = y1 + (userSpeed - x1) * (y2 - y1) / (x2 - x1);
                break;
            }
        }
    }
    return interpolatedValue;
}