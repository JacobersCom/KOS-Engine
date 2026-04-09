#include "KOSVulkanPrerequisites.h"

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include "vulkan/vulkan_win32.h"
#endif

class KOSVulkanRenderer;

/*
KOSVulkanWindow depends on KOSVulkanRenderer to be enabled first
KOSVulkanWindow handles everything that is presented to the window
*/
class KOSVulkanWindow {
public:

	const char* KWindowTitle;
	int KWidth, KHeight;
	bool KActive;

	KOSVulkanWindow(const char* WindowTitle, int width, int height);

	/*
	Used to created a surface for the rendering system
	*/
	void CreateSurface(size_t windowhandle);

private:

};