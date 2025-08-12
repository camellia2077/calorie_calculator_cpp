# 1. 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.15)

# 2. 定义项目名称和所用语言
project(WorkoutCalculator LANGUAGES CXX)

# 3. 设置C++标准为 C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 4. (*** 主要修改部分 ***)
#    为每个包含源文件的文件夹分别收集 .cpp 文件
file(GLOB MAIN_SRC "src/main.cpp")
file(GLOB APP_SRC "src/app/*.cpp")
file(GLOB CORE_SRC "src/core/*.cpp")
file(GLOB UI_SRC "src/ui/*.cpp")

#    添加可执行文件目标，并传入上面找到的所有源文件
add_executable(calorie_calculator
    ${MAIN_SRC}
    ${APP_SRC}
    ${CORE_SRC}
    ${UI_SRC}
)

# 5. 指定所有头文件所在的根目录
#    这使得在代码中可以直接使用 #include "app/Application.h" 这样的路径
#    或者 #include "Application.h" (如果编译器配置了子目录为include路径)
target_include_directories(calorie_calculator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/domain
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# 6. 为目标强制添加编译和链接选项 (无需修改)
target_compile_options(calorie_calculator PRIVATE -Os)
target_link_options(calorie_calculator PRIVATE -s)