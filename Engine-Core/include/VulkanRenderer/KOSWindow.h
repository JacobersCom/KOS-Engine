
#include "Windows.h"
/*
KOSVulkanWindow depends on KOSVulkanRenderer to be enabled first
KOSVulkanWindow handles everything that is presented to the window
*/
class KOSWindow {
public:


	KOSWindow(const char* WindowTitle, int width, int height);

	const bool shouldClose() { return windowState; };
	bool PollEvents();

private:

	void CreateWin32();

	const char* windowTitle;
	int width, height;

	bool windowState;

	HWND windowHandle;

};