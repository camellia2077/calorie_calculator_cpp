# 1. 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.15)

# 2. 定义项目名称和所用语言
project(WorkoutCalculator LANGUAGES CXX)

# 3. 设置C++标准为 C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 4. 添加可执行文件目标 (*** 这是主要修改部分 ***)
#    移除了旧的 WorkoutCalculator.cpp
#    添加了新的 Application.cpp, CalculationEngine.cpp, 和 ConsoleUI.cpp
add_executable(calorie_calculator
    src/main.cpp
    src/Application.cpp
    src/CalculationEngine.cpp
    src/ConsoleUI.cpp
)

# 5. 指定头文件所在的目录 (无需修改)
#    所有头文件仍然在 src 目录下
target_include_directories(calorie_calculator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 6. 为目标强制添加编译和链接选项 (无需修改)
#    这些优化选项依然适用
target_compile_options(calorie_calculator PRIVATE -Os)
target_link_options(calorie_calculator PRIVATE -s)