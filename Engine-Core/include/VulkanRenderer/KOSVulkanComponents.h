#pragma once


#include <vulkan/vulkan.h>

#ifdef _WIN32

#define WINDOWS_LEAN_AND_MEAN
#define VK_USE_PLATFORM_WIN32_KHR

#include "Windows.h"
#include "vulkan/vulkan_win32.h"

#endif

//deps
#include <VkBootstrap.h>

#include <thread>
#include <condition_variable>
#include <mutex>

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
		HWND WindowHandle;
		VkSurfaceKHR Win32Surface;
		
	};

	struct RenderState
	{
		std::mutex sharedLock;
		std::condition_variable cv;

		bool instanceReady = false;
		bool surfaceReady = false;
		bool shutdown = false;
	};

	struct RenderSync
	{
		//Allows there to be on sychronizaed state instead of copying states
		std::shared_ptr<RenderState> state = std::make_shared<RenderState>();
	};

}
