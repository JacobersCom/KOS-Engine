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
	
	VkWin32SurfaceCreateInfoKHR SurInfo{};

	SurInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	SurInfo.hwnd = (HWND)windowhandle;
	SurInfo.hinstance = GetModuleHandle(NULL);

	try
	{
		if(vkCreateWin32SurfaceKHR())
	}
#endif // _WIN32

	
}
