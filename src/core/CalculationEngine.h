#ifndef CALCULATION_ENGINE_H
#define CALCULATION_ENGINE_H

#include <vector>
#include "domain/DataPoint.h"
#include "domain/WorkoutDomain.h" 
#include "core/kinematics/PaceCalculator.h"        // <--- 关键：引用子计算器的头文件
#include "core/physiology/MetCalorieCalculator.h"  // <--- 关键：引用子计算器的头文件

/**
 * @brief 一个统一的计算引擎门面类。
 * 封装了所有子计算器的调用逻辑，为外部提供一个简单的接口。
 */
class CalculationEngine {
public:
    /**
     * @brief 执行所有运动指标和生理指标的计算。
     * @param params 包含用户输入的原始参数。
     * @param dataTable 包含特定运动的(速度, METs)数据。
     * @return WorkoutResults 一个包含所有最终计算结果的结构体。
     */
    WorkoutResults calculate(const WorkoutParameters& params, const std::vector<DataPoint>& dataTable) const;

private:
    // 将子计算器作为私有成员，封装实现细节
    PaceCalculator paceCalculator_;
    MetCalorieCalculator metCalorieCalculator_;
};

#endif // CALCULATION_ENGINE_H