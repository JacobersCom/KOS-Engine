#include "VulkanRenderer/KOSVulkanWindow.h"
#include "VulkanRenderer/KOSVulkanSystems.h"

#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan_win32.h"
#include "Windows.h"
#endif

//ios
#include <iostream>

KOSVulkanWindow::KOSVulkanWindow(const char* WindowTitle, int width, int height)
	: KWindowTitle(WindowTitle), KWidth(width), KHeight(height)
{
	KActive = true;
}

void KOSVulkanWindow::CreateSurface(size_t windowhandle)
{
#ifdef _WIN32
	
	std::unique_ptr<KOSVulkanRenderer> renderer = std::make_unique<KOSVulkanRenderer>();
	auto& Instance = renderer->KInstance;

	 Surface = VK_NULL_HANDLE;

	VkWin32SurfaceCreateInfoKHR SurInfo{};

	SurInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	SurInfo.hwnd = (HWND)windowhandle;
	SurInfo.hinstance = GetModuleHandle(NULL);

	
	VkResult result = vkCreateWin32SurfaceKHR(Instance, &SurInfo, nullptr, &Surface);

#if _DEBUG
	if (result == VK_SUCCESS)
	{
		printf("Win32 Surface created");
	}
	else
	{
		printf("Failed to create Win32 Surface");
	}
#endif // _DEBUG

#endif // _WIN32

	
}
