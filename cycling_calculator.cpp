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


// --- 创建一个结构体来统一管理所有数据 ---
struct WorkoutData {
    // 用户输入
    double timeHr = 0, timeMin = 0, timeSec = 0;
    double distanceKm = 0, weightKg = 0;

    // 计算出的值
    double totalTimeInMinutes = 0;
    double userSpeedKmh = 0;
    double speedMps = 0;
    int paceMinutes = 0, paceSeconds = 0;
    double averageMets = 0;
    double totalKcal = 0, totalKj = 0;
    int equivalentHours = 0, equivalentMinutes = 0;
};

// --- 函数声明 ---
void getUserInput(WorkoutData& data);
bool calculateResults(WorkoutData& data);
void displayResults(const WorkoutData& data);
double getInterpolatedValue(double userSpeed, const std::vector<double>& x_data, const std::vector<double>& y_data);


// =================================================================
// 主函数 (main) - 现在的“指挥中心”
// =================================================================
int main() {
    // 1. 环境设置
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif
    std::cout << std::fixed << std::setprecision(2);

    // 2. 创建数据对象
    WorkoutData workout;

    // 3. 调用函数获取输入
    getUserInput(workout);

    // 4. 调用函数进行计算，并根据计算是否成功来决定是否显示结果
    if (calculateResults(workout)) {
        displayResults(workout);
    }

    return 0;
}


// =================================================================
// 功能函数实现
// =================================================================

/**
 * @brief 获取用户的骑行数据
 * @param data 通过引用传递，函数将直接修改传入的WorkoutData对象
 */
void getUserInput(WorkoutData& data) {
    std::cout << "请输入您的骑行时间（小时部分）: ";
    std::cin >> data.timeHr;
    std::cout << "请输入您的骑行时间（分钟部分）: ";
    std::cin >> data.timeMin;
    std::cout << "请输入您的骑行时间（秒的部分）: ";
    std::cin >> data.timeSec;

    std::cout << "请输入您的骑行距离（公里）: ";
    std::cin >> data.distanceKm;

    std::cout << "请输入您的体重（公斤）: ";
    std::cin >> data.weightKg;
}

/**
 * @brief 执行所有计算，并进行数据验证
 * @param data 通过引用传递，函数将填充所有计算结果字段
 * @return bool 如果计算和验证成功，返回true；如果速度超范围，返回false
 */
bool calculateResults(WorkoutData& data) {
    // 输入验证
    if (data.timeHr < 0 || data.timeMin < 0 || data.timeMin >= 60 || data.timeSec < 0 || data.timeSec >= 60 || data.distanceKm <= 0 || data.weightKg <= 0) {
        std::cerr << "错误：输入值不合法。时间不能为负，分钟和秒数需小于60，距离和体重必须为正数。" << std::endl;
        return false;
    }

    // 计算总时间
    data.totalTimeInMinutes = (data.timeHr * 60.0) + data.timeMin + (data.timeSec / 60.0);
    
    // 计算速度
    data.userSpeedKmh = (data.totalTimeInMinutes > 0) ? (data.distanceKm / (data.totalTimeInMinutes / 60.0)) : 0;
    
    // 速度范围检查
    if (data.userSpeedKmh < speedsKph.front() || data.userSpeedKmh > speedsKph.back()) {
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "您的平均速度是: " << data.userSpeedKmh << " 公里/小时 (km/h)" << std::endl;
        std::cout << "\n错误：您的速度不在有效计算范围内。" << std::endl;
        std::cout << "本程序可计算的速度范围为 " << speedsKph.front() << " 到 " << speedsKph.back() << " 公里/小时。" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        return false;
    }

    // 计算其他衍生值
    data.speedMps = data.userSpeedKmh / 3.6;
    double paceDecimal = (data.userSpeedKmh > 0) ? (60.0 / data.userSpeedKmh) : 0;
    data.paceMinutes = static_cast<int>(paceDecimal);
    data.paceSeconds = static_cast<int>(round((paceDecimal - data.paceMinutes) * 60.0));
    
    // 核心热量计算
    data.averageMets = getInterpolatedValue(data.userSpeedKmh, speedsKph, mets);
    data.totalKcal = data.averageMets * data.weightKg * (data.totalTimeInMinutes / 60.0);
    const double KJ_PER_KCAL = 4.184;
    data.totalKj = data.totalKcal * KJ_PER_KCAL;
    
    // 等效时间计算
    double equivalentRestingMinutes = data.averageMets * data.totalTimeInMinutes;
    data.equivalentHours = static_cast<int>(equivalentRestingMinutes / 60);
    data.equivalentMinutes = static_cast<int>(round(fmod(equivalentRestingMinutes, 60.0)));
    
    return true;
}

/**
 * @brief 将计算结果格式化并显示给用户
 * @param data 通过const引用传递，保证函数不会修改数据
 */
void displayResults(const WorkoutData& data) {
    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "您的平均速度是: " << data.userSpeedKmh << " 公里/小时 (km/h)" << std::endl;
    std::cout << "               " << "(" << data.speedMps << " 米/秒 (m/s))" << std::endl;
    std::cout << "您的平均配速是: " << data.paceMinutes << "分" << data.paceSeconds << "秒 / 公里" << std::endl;

    std::cout << "\n计算结果:" << std::endl;
    std::cout << "本次骑行平均代谢当量 (METs): " << data.averageMets << std::endl;
    std::cout << "您消耗的总热量约为: " << data.totalKj << " 千焦 (kJ)" << std::endl;
    std::cout << "                  " << "或 " << data.totalKcal << " 千卡/大卡 (kcal)" << std::endl;
    std::cout << "\n从消耗热量来看，本次骑行相当于静坐了：" << std::endl;
    std::cout << data.equivalentHours << " 小时 " << data.equivalentMinutes << " 分钟" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
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
    } else {
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