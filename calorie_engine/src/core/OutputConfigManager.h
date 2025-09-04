#ifndef OUTPUT_CONFIG_MANAGER_H
#define OUTPUT_CONFIG_MANAGER_H

#include <string>

// 定义一个结构体来保存输出配置
struct OutputConfiguration {
    bool showWorkoutPerformance;
    bool showCalorieConsumption;
    bool showFoodEquivalents;
    bool showEquivalentActivity;
};

class OutputConfigManager {
public:
    // 构造函数：加载并解析JSON配置文件
    OutputConfigManager(const std::string& jsonFilePath);

    // 获取加载后的输出配置
    const OutputConfiguration& getOutputConfig() const;

private:
    OutputConfiguration config_;

    // 私有辅助函数，用于加载和解析JSON
    void loadAndParseJson(const std::string& filePath);
};

#endif // OUTPUT_CONFIG_MANAGER_H