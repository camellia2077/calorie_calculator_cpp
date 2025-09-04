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

# ==================== 修改开始 ====================

# 新增: 查找 SQLite3 库
find_package(SQLite3 REQUIRED) # <--- ADDED

# 4. 创建一个核心静态库，并显式列出所有共享源文件
add_library(workout_lib STATIC
    src/app/Application.cpp

    src/db_inserter/DatabaseManager.cpp
    src/db_inserter/DbInserterFacade.cpp
    src/db_inserter/JsonImporter.cpp
    
    src/core/CalculationEngine.cpp
    src/core/FoodConverter.cpp
    src/core/FoodDataManager.cpp
    src/core/OutputConfigManager.cpp

    src/core/kinematics/PaceCalculator.cpp
    src/core/physiology/MetCalorieCalculator.cpp
    src/core/math/Interpolator.cpp

    src/ui/OutputFormat.cpp
)

# 5. 将通用配置应用到核心库上
target_include_directories(workout_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_precompile_headers(workout_lib PUBLIC <pch.h>) 

# stdc++exp是为了<print>
# 将 stdc++exp 和 SQLite3 链接到库
# 使用 PUBLIC 关键字，这样可执行文件也会正确链接
target_link_libraries(workout_lib PUBLIC stdc++exp SQLite::SQLite3) # <--- MODIFIED

# 6. 定义独立的可执行文件目标 (只保留CLI版本)
add_executable(calorie_calculator_cli src/main_cli.cpp) 

# 7. 将可执行文件链接到核心库
target_link_libraries(calorie_calculator_cli PRIVATE workout_lib) 

# ==================== 修改结束 ====================

# 8. 为所有目标添加通用的编译和链接选项
set(COMMON_COMPILE_OPTIONS "-fdiagnostics-color=always")
set(RELEASE_COMPILE_OPTIONS -Os)
set(RELEASE_LINK_OPTIONS -s)

# 将编译选项应用到所有目标
target_compile_options(workout_lib PRIVATE ${COMMON_COMPILE_OPTIONS}) 
target_compile_options(calorie_calculator_cli PRIVATE ${COMMON_COMPILE_OPTIONS}) 

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Release mode build: Applying size optimization flags (-Os, -s).")
    target_compile_options(workout_lib PRIVATE ${RELEASE_COMPILE_OPTIONS}) 
    target_compile_options(calorie_calculator_cli PRIVATE ${RELEASE_COMPILE_OPTIONS}) 
    
    target_link_options(calorie_calculator_cli PRIVATE ${RELEASE_LINK_OPTIONS}) 
endif()
