# 1. 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.16)

# 2. 定义项目名称和所用语言
project(WorkoutCalculator LANGUAGES CXX)

# 3. 设置C++标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- 启用 CCACHE (如果已安装) ---
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "ccache found, setting up compiler launcher.")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_FOUND}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_FOUND}")
else()
    message(STATUS "ccache not found.")
endif()

# 4. 收集所有共享的源文件 (除了两个 main 函数)
file(GLOB_RECURSE COMMON_SOURCES
    "src/app/*.cpp"
    "src/core/*.cpp"
    "src/core/kinematics/*.cpp"
    "src/core/physiology/*.cpp"
    "src/core/math/*.cpp"
    "src/ui/*.cpp"
)

# 5. 定义两个独立的可执行文件目标
add_executable(calorie_calculator src/main.cpp ${COMMON_SOURCES})
add_executable(calorie_calculator_cli src/main_cli.cpp ${COMMON_SOURCES})

# 6. 为两个目标指定头文件根目录
target_include_directories(calorie_calculator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(calorie_calculator_cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 7. 为两个目标设置预编译头文件
target_precompile_headers(calorie_calculator PRIVATE <common/pch.h>)
target_precompile_headers(calorie_calculator_cli PRIVATE <common/pch.h>)

# 8. 为两个目标添加通用的编译和链接选项
set(COMMON_COMPILE_OPTIONS "-fdiagnostics-color=always")
set(RELEASE_COMPILE_OPTIONS -Os)
set(RELEASE_LINK_OPTIONS -s)

target_compile_options(calorie_calculator PRIVATE ${COMMON_COMPILE_OPTIONS})
target_compile_options(calorie_calculator_cli PRIVATE ${COMMON_COMPILE_OPTIONS})

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Release mode build: Applying size optimization flags (-Os, -s).")
    target_compile_options(calorie_calculator PRIVATE ${RELEASE_COMPILE_OPTIONS})
    target_compile_options(calorie_calculator_cli PRIVATE ${RELEASE_COMPILE_OPTIONS})
    target_link_options(calorie_calculator PRIVATE ${RELEASE_LINK_OPTIONS})
    target_link_options(calorie_calculator_cli PRIVATE ${RELEASE_LINK_OPTIONS})
endif()