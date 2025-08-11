// 重要提示：请务必将此源文件保存为 UTF-8 编码格式！
#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip> // 用于设置输出精度
#include <cmath>   // 用于 floor 函数

// 为 Windows 平台引入头文件，用于设置控制台编码
#ifdef _WIN32
#include <windows.h>
#endif

// --- 将数据定义为全局常量，方便主函数访问范围 ---
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
// y: 能量消耗率 (千焦/小时/公斤)
const std::vector<double> kjPerHourPerKg = {
    33.0, 42.0, 46.0, 48.0, 52.0, 56.0, 59.0, 63.0, 67.0, 75.0
};

// 函数声明
double calculateEnergyExpenditure(double totalTimeMin, double distanceKm, double weightKg);

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

    // 进行输入验证
    if (timeMin < 0 || timeSec < 0 || timeSec >= 60 || distanceKm <= 0 || weightKg <= 0) {
        std::cerr << "错误：输入值不合法。时间不能为负，秒数需小于60，距离和体重必须为正数。" << std::endl;
        return 1;
    }

    double totalTimeInMinutes = timeMin + (timeSec / 60.0);
    
    // 提前计算速度 (km/h)
    double userSpeedKmh = (totalTimeInMinutes > 0) ? (distanceKm / (totalTimeInMinutes / 60.0)) : 0;
    
    // --- 新增：计算其他速度和配速单位 ---
    double speedMps = userSpeedKmh / 3.6;
    double paceDecimal = (userSpeedKmh > 0) ? (60.0 / userSpeedKmh) : 0;
    int paceMinutes = static_cast<int>(paceDecimal);
    int paceSeconds = static_cast<int>(round((paceDecimal - paceMinutes) * 60.0));
    // ---

    std::cout << "\n----------------------------------------" << std::endl;
    // --- 修改后的速度/配速输出 ---
    std::cout << "您的平均速度是: " << userSpeedKmh << " 公里/小时 (km/h)" << std::endl;
    std::cout << "               " << "(" << speedMps << " 米/秒 (m/s))" << std::endl;
    std::cout << "您的平均配速是: " << paceMinutes << "分" << paceSeconds << "秒 / 公里" << std::endl;
    // ---

    // 速度范围检查
    if (userSpeedKmh < speedsKph.front() || userSpeedKmh > speedsKph.back()) {
        std::cout << "\n错误：您的速度不在有效计算范围内。" << std::endl;
        std::cout << "本程序可计算的速度范围为 " << speedsKph.front() << " 到 " << speedsKph.back() << " 公里/小时。" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return 1;
    }

    // 调用函数计算总消耗热量
    double totalKj = calculateEnergyExpenditure(totalTimeInMinutes, distanceKm, weightKg);
    const double KJ_TO_KCAL_CONVERSION_FACTOR = 4.184;
    double totalKcal = totalKj / KJ_TO_KCAL_CONVERSION_FACTOR;

    // 输出结果
    std::cout << "\n计算结果:" << std::endl;
    std::cout << "您消耗的总热量约为: " << totalKj << " 千焦 (kJ)" << std::endl;
    std::cout << "                  " << "或 " << totalKcal << " 千卡/大卡 (kcal)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    return 0;
}

double calculateEnergyExpenditure(double totalTimeMin, double distanceKm, double weightKg) {
    double userSpeedKph = distanceKm / (totalTimeMin / 60.0);
    double interpolatedKjRate = 0.0;
    
    if (userSpeedKph == speedsKph.back()) {
        interpolatedKjRate = kjPerHourPerKg.back();
    } else {
        for (size_t i = 0; i < speedsKph.size() - 1; ++i) {
            if (userSpeedKph >= speedsKph[i] && userSpeedKph < speedsKph[i + 1]) {
                double x1 = speedsKph[i];
                double y1 = kjPerHourPerKg[i];
                double x2 = speedsKph[i + 1];
                double y2 = kjPerHourPerKg[i + 1];
                interpolatedKjRate = y1 + (userSpeedKph - x1) * (y2 - y1) / (x2 - x1);
                break;
            }
        }
    }
    
    double totalKj = interpolatedKjRate * weightKg * (totalTimeMin / 60.0);
    return totalKj;
}