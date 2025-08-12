#ifndef WORKOUT_DOMAIN_H
#define WORKOUT_DOMAIN_H

// 这个新文件用于定义多个类之间共享的数据结构，避免了循环依赖


// 用于存储用户输入的原始数据
struct WorkoutParameters {
    double timeHr, timeMin, timeSec;
    double distanceKm;
    double weightKg;
};

// 用于存储所有计算结果
struct WorkoutResults {
    double totalTimeInMinutes;
    double userSpeedKmh;
    double speedMps;
    int paceMinutes;
    int paceSeconds;
    double averageMets;
    double totalKcal;
    double totalKj;
    int equivalentHours;
    int equivalentMinutes;
};

#endif // WORKOUT_DOMAIN_H