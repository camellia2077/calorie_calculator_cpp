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
// 数据来源: Compendium of Physical Activities (https://pacompendium.com/)
// 原始单位为 mph (英里/小时)，已转换为 km/h (公里/小时)
// 转换公式: 1 mph = 1.60934 km/h
const std::vector<double> speedsKph = {
    6.60,  // 4.1 mph (avg of 4-4.2)
    7.32,  // 4.55 mph (avg of 4.3-4.8)
    8.21,  // 5.1 mph (avg of 5.0-5.2)
    9.09,  // 5.65 mph (avg of 5.5-5.8)
    9.90,  // 6.15 mph (avg of 6-6.3)
    10.78, // 6.7 mph
    11.27, // 7 mph
    12.07, // 7.5 mph
    12.87, // 8 mph
    13.84, // 8.6 mph
    14.48, // 9 mph
    15.21, // 9.45 mph (avg of 9.3-9.6)
    16.09, // 10 mph
    17.70, // 11 mph
    19.31, // 12 mph
    20.92, // 13 mph
    22.53  // 14 mph
};

const std::vector<double> mets = {
    6.5,
    7.8,
    8.5,
    9.0,
    9.3,
    10.5,
    11.0,
    11.8,
    12.0,
    12.5,
    13.0,
    14.8, // 对应 9.45 mph
    14.8, // 对应 10 mph
    16.8,
    18.5,
    19.8,
    23.0
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

    double timeMin = 0, timeSec = 0, distanceKm = 0, weightKg = 0;

    // --- 输入部分 ---
    std::cout << "请输入您的跑步时间（分钟部分）: ";
    std::cin >> timeMin;
    std::cout << "请输入您的跑步时间（秒的部分）: ";
    std::cin >> timeSec;

    std::cout << "请输入您的跑步距离（公里）: ";
    std::cin >> distanceKm;

    std::cout << "请输入您的体重（公斤）: ";
    std::cin >> weightKg;

    // 输入验证
    if (timeMin < 0 || timeSec < 0 || timeSec >= 60 || distanceKm <= 0 || weightKg <= 0) {
        std::cerr << "错误：输入值不合法。时间不能为负，秒数需小于60，距离和体重必须为正数。" << std::endl;
        return 1;
    }

    double totalTimeInMinutes = timeMin + (timeSec / 60.0);
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
    
    // ++++++++++ 主要计算逻辑 ++++++++++
    double averageMets = getInterpolatedValue(userSpeedKmh, speedsKph, mets);
    double totalKcal = averageMets * weightKg * (totalTimeInMinutes / 60.0);
    const double KJ_PER_KCAL = 4.184;
    double totalKj = totalKcal * KJ_PER_KCAL;
    double equivalentRestingMinutes = averageMets * totalTimeInMinutes;
    int equivalentHours = static_cast<int>(equivalentRestingMinutes / 60);
    int equivalentMinutesPart = static_cast<int>(round(fmod(equivalentRestingMinutes, 60.0)));

    // --- 结果输出部分 ---
    std::cout << "\n计算结果:" << std::endl;
    std::cout << "本次运动平均代谢当量 (METs): " << averageMets << std::endl;
    std::cout << "您消耗的总热量约为: " << totalKj << " 千焦 (kJ)" << std::endl;
    std::cout << "                  " << "或 " << totalKcal << " 千卡/大卡 (kcal)" << std::endl;
    std::cout << "\n从消耗热量来看，本次跑步相当于静坐了：" << std::endl;
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