#ifndef PCH_H
#define PCH_H

// --- C++ 标准库 ---
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <limits>
#include <algorithm> // 这是一个常用的头文件，可以加进来

// --- 项目内部稳定且被广泛使用的头文件 ---
// 这些定义了核心数据结构，很适合放入 PCH
#include "domain/DataPoint.h"
#include "domain/WorkoutDomain.h"
#include "domain/FoodEquivalent.h"

// --- Windows 平台特定头文件 (如果需要跨平台) ---
#ifdef _WIN32
#include <windows.h>
#endif

#endif //PCH_H