#ifndef APPLICATION_H
#define APPLICATION_H

#include "ui/OutputFormat.h"
#include "core/CalculationEngine.h"
#include "core/FoodConverter.h"
#include "core/FoodDataManager.h"
#include "core/OutputConfigManager.h"
#include "domain/WorkoutDomain.h"
#include "db_inserter/DbInserterFacade.h"

#include <string>

class Application {
public:
    Application(const std::string& foodDataPath, const std::string& outputConfigPath);

    void runOnce(const std::string& sportType, const WorkoutParameters& params);

    void importActivityFromJson(const std::string& inputPath, const std::string& dbPath);

private:
    OutputFormat ui;
    CalculationEngine engine;
    FoodConverter foodConverter;
    FoodDataManager foodDataManager_;
    OutputConfigManager outputConfigManager_;

    bool validateParameters(const WorkoutParameters& params);
};

#endif // APPLICATION_H
