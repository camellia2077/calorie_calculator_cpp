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
// x: 速度 (公里/小时)
const std::vector<double> speedsKph = {
    60.0 / 7.5,  // 8.00
    60.0 / 6.25, // 9.60
    60.0 / 5.5,  // 10.91
    60.0 / 5.3,  // 11.32
    60.0 / 5.0,  // 12.00
    60.0 / 4.7,  // 12.77
    60.0 / 4.3,  // 13.95
    60.0 / 4.1,  // 14.63
    60.0 / 3.7,  // 16.22
    60.0 / 3.4   // 17.65
};
// y: 代谢当量 (METs) - 现在是唯一的Y值数据
const std::vector<double> mets = {
    8.0, 10.0, 11.0, 11.5, 12.5, 13.5, 14.0, 15.0, 16.0, 18.0
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
    
    // ++++++++++ 主要计算逻辑（已更新）++++++++++
    // 1. 获取运动的平均METs值
    double averageMets = getInterpolatedValue(userSpeedKmh, speedsKph, mets);
    
    // 2. 根据METs核心公式计算总千卡(kcal)
    // 核心公式: 总热量(kcal) = METs * 体重(kg) * 时间(小时)
    double totalKcal = averageMets * weightKg * (totalTimeInMinutes / 60.0);
    
    // 3. 将千卡换算为千焦(kJ)
    const double KJ_PER_KCAL = 4.184;
    double totalKj = totalKcal * KJ_PER_KCAL;
    
    // 4. 计算等效静坐时间
    double equivalentRestingMinutes = averageMets * totalTimeInMinutes;
    int equivalentHours = static_cast<int>(equivalentRestingMinutes / 60);
    int equivalentMinutesPart = static_cast<int>(round(fmod(equivalentRestingMinutes, 60.0)));
    // +++++++++++++++++++++++++++++++++++++++++++

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
 * @param userSpeed 用户的速度 (km/h)
 * @param x_data 速度数据表 (横坐标)
 * @param y_data 需要计算的Y值数据表 (纵坐标)
 * @return double 插值计算后的Y值
 */
double getInterpolatedValue(double userSpeed, const std::vector<double>& x_data, const std::vector<double>& y_data) {
    double interpolatedValue = 0.0;
    
    if (userSpeed >= x_data.back()) { // 大于等于最大速度
        interpolatedValue = y_data.back();
    } else if (userSpeed <= x_data.front()) { // 小于等于最小速度
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