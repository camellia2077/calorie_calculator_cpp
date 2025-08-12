#ifndef FOOD_CONVERTER_H
#define FOOD_CONVERTER_H

#include <vector>
#include <map>
#include <string>
#include "domain/FoodEquivalent.h"
#include "WorkoutDomain.h"

class FoodConverter {
public:
    // 接收总热量，返回一个按类别组织的食物等效列表
    std::map<std::string, std::vector<FoodEquivalent>> calculate(double totalKcal);
};

#endif // FOOD_CONVERTER_H