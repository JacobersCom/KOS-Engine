#pragma once


#include <vulkan/vulkan.h>

#ifdef _WIN32

#define WINDOWS_LEAN_AND_MEAN
#define VK_USE_PLATFORM_WIN32_KHR

#include "Windows.h"
#include "vulkan/vulkan_win32.h"

#endif

#include <VkBootstrap.h>

namespace RENDERER
{
	//Everything to init vulkan
	struct VulkanData
	{
		vkb::Instance vkbIns;
		vkb::PhysicalDevice vkbPhy;
		vkb::Device vkbDev;
		vkb::Swapchain vkbSwapchain;
	};

	struct Surface
	{
		VkSurfaceKHR Win32Surface;
	};

}
