#pragma once

#include "Common.h"

#if defined(_WIN32) || defined(_WIN64)
#include<vulkan/vulkan_win32.h>
global const bool windows = true;
#elif
global const bool windows = false;
#endif 
#include <vulkan/vulkan.h>

#include <vector>
#include <optional>

