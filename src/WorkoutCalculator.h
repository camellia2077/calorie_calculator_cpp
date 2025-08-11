#ifndef WORKOUTCALCULATOR_H
#define WORKOUTCALCULATOR_H

#include <string>
#include <vector>
#include "DataPoint.h" // 需要知道DataPoint是什么

// 声明WorkoutCalculator类
class WorkoutCalculator {
public:
    // 构造函数：在创建对象时，必须提供运动名称和对应的数据表
    WorkoutCalculator(const std::string& name, const std::vector<DataPoint>& data);

    // 公共接口：运行计算器的主流程
    void run();

private:
    // --- 成员变量 ---
    std::string activityName;                   // 存储运动名称，如 "跑步" 或 "骑行"
    const std::vector<DataPoint>& dataTable;    // 对数据表的引用，高效且能访问外部数据
    
    struct WorkoutInput { // 用于存储单次计算的用户输入和结果
        double timeHr, timeMin, timeSec, distanceKm, weightKg;
        double totalTimeInMinutes, userSpeedKmh, speedMps;
        int paceMinutes, paceSeconds;
        double averageMets, totalKcal, totalKj;
        int equivalentHours, equivalentMinutes;
    };
    WorkoutInput currentWorkout; // 存储当前计算的数据

    // --- 私有辅助函数 ---
    void getUserInput();
    bool calculateResults();
    void displayResults();
    double getInterpolatedValue(double userSpeed);
};

#endif // WORKOUTCALCULATOR_H