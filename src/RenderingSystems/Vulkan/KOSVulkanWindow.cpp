#include "RenderingSystems/Vulkan/KOSVulkanWindow.h"
#include "RenderingSystems/Vulkan/KOSVulkanRenderer.h"

KOSVulkanWindow::KOSVulkanWindow(const char* WindowTitle, int width, int height)
	: KWindowTitle(WindowTitle), KWidth(width), KHeight(height)
{
	KActive = true;
}

void KOSVulkanWindow::CreateSurface(size_t windowhandle)
{
#ifdef _WIN32
	
	std::unique_ptr<KOSVulkanRenderer> renderer = std::make_unique<KOSVulkanRenderer>();

	VkSurfaceKHR Surface = VK_NULL_HANDLE;

	VkWin32SurfaceCreateInfoKHR SurInfo{};

	SurInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	SurInfo.hwnd = (HWND)windowhandle;
	SurInfo.hinstance = GetModuleHandle(NULL);

	
	VkResult result = vkCreateWin32SurfaceKHR(renderer->KInstance, &SurInfo, nullptr, &Surface);
	if (result == VK_SUCCESS)
	{
		printf("Win32 Surface created");
	}
	else
	{
		printf("Failed to create Win32 Surface");
	}
#endif // _WIN32

	
}
