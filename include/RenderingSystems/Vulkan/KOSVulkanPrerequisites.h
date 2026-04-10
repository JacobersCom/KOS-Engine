#include <vulkan/vulkan.h>

#include <vector>
#include <>


//If on windows using vulkan
#ifdef _WIN32
#include "vulkan/vulkan_win32.h"
#include "Windows.h"
#define VK_USE_PLATFORM_WIN32_KHR
#define WINDOWS_LEAN_AND_MEAN
#endif

	