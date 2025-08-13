#ifndef WORKOUT_DOMAIN_H
#define WORKOUT_DOMAIN_H

// This file defines data structures shared between multiple classes.

// Stores the original user input
struct WorkoutParameters {
    double timeHr, timeMin, timeSec;
    double distanceKm;
    double weightKg;
};

// A unified structure containing all final calculation results
struct WorkoutResults {
    // --- Results from PaceCalculator ---
    double totalTimeInMinutes;
    double userSpeedKmh;
    double speedMps;
    int paceMinutes;
    double paceSeconds; // <--- The Fix: Change this from int to double

    // --- Results from MetCalorieCalculator ---
    double averageMets;
    double totalKcal;
    double totalKj;
    int equivalentHours;
    int equivalentMinutes;
};

#endif // WORKOUT_DOMAIN_H