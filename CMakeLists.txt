# 1. 设置CMake的最低版本要求 (建议 3.16+ 以获得更好的PCH支持)
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

# 4. 收集所有源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 添加可执行文件目标
add_executable(calorie_calculator ${SOURCES})

# 5. 指定头文件根目录
target_include_directories(calorie_calculator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 6. (*** 修正：添加预编译头文件 ***)
target_precompile_headers(calorie_calculator
    PRIVATE
        # 将 pch.h 指定为预编译头文件
        # PRIVATE 表示它只为 calorie_calculator 目标本身预编译
    # 而不是让链接到它的其他库也使用这个PCH
        <common/pch.h> # <--- FIX: Update the path here
)

# 7. 添加编译和链接选项
target_compile_options(calorie_calculator PRIVATE "-fdiagnostics-color=always")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Release mode build: Applying size optimization flags (-Os, -s).")
    target_compile_options(calorie_calculator PRIVATE -Os)
    target_link_options(calorie_calculator PRIVATE -s)
endif()