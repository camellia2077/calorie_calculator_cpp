#ifndef WORKOUT_DOMAIN_H
#define WORKOUT_DOMAIN_H

// 这个文件定义了多个类之间共享的数据结构

// 用于存储用户输入的原始数据
struct WorkoutParameters {
    double timeHr, timeMin, timeSec;
    double distanceKm;
    double weightKg;
};

// (*** 这是主要修改点 ***)
// 一个统一的、包含所有计算结果的结构体，供最终使用
struct WorkoutResults {
    // --- PaceCalculator 的结果 ---
    double totalTimeInMinutes;
    double userSpeedKmh;
    double speedMps;
    int paceMinutes;
    int paceSeconds;
    // --- MetCalorieCalculator 的结果 ---
    double averageMets;
    double totalKcal;
    double totalKj;
    int equivalentHours;
    int equivalentMinutes;
};

#endif // WORKOUT_DOMAIN_H