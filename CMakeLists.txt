# 1. 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.15)

# 2. 定义项目名称和所用语言
project(WorkoutCalculator LANGUAGES CXX)

# 3. 设置C++标准为 C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 4. 添加可执行文件目标
add_executable(calorie_calculator
    src/main.cpp
    src/WorkoutCalculator.cpp
)

# 5. 指定头文件所在的目录 (修改为绝对路径)
# ${CMAKE_CURRENT_SOURCE_DIR} 是一个CMake变量，代表当前CMakeLists.txt文件所在的绝对路径
target_include_directories(calorie_calculator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 6. 为目标强制添加编译和链接选项 (新增部分)
# -Os 是一个编译选项，用于优化代码尺寸
target_compile_options(calorie_calculator PRIVATE -Os)
# -s 是一个链接选项，用于移除符号表，减小最终文件体积
target_link_options(calorie_calculator PRIVATE -s)