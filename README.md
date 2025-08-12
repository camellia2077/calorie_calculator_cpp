# run_calorie_calculator
计算跑步运动消耗热量

## 目录
```
src/
├── app/                  # 应用程序核心逻辑与流程控制
│   ├── Application.cpp   # Application 类的实现，负责协调UI、计算引擎等模块
│   └── Application.h     # Application 类的头文件，定义程序的主应用流程
│
├── core/                 # 核心业务逻辑与计算引擎
│   ├── CalculationEngine.cpp  # 计算引擎的实现，处理所有核心数学运算（如METs插值、卡路里计算）
│   ├── CalculationEngine.h    # 计算引擎的头文件，声明核心计算功能
│   ├── FoodConverter.cpp      # 食物等效转换器的实现，负责将消耗的卡路里换算成等量的食物
│   └── FoodConverter.h        # 食物等效转换器的头文件，声明卡路里到食物的转换功能
│
├── data/                 # 存放所有静态数据
│   ├── cycling_data.h    # 骑行运动的 METs（代谢当量）数据表
│   ├── food_data.h       # 常见食物的热量数据
│   └── running_data.h    # 跑步运动的 METs（代谢当量）数据表
│
├── domain/               # 定义项目中的核心数据结构（Plain Old Data Structures）
│   ├── DataPoint.h       # 定义了通用的数据点结构（速度 和 METs），用于存储运动数据
│   ├── FoodEquivalent.h  # 定义了食物等效换算结果的数据结构
│   └── WorkoutDomain.h   # 定义了运动相关的核心数据结构，
│
├── main.cpp              # 程序的主入口文件，负责启动应用程序
│
└── ui/                   # 用户界面（User Interface）相关代码
    ├── ConsoleUI.cpp     # 控制台界面的实现，处理所有用户输入（cin）和输出（cout）
    └── ConsoleUI.h       # 控制台界面的头文件，声明所有与用户交互的函数
```
