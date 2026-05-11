#pragma once

#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#include "Windows.h"
#endif

//deps
#include <entt/entt.hpp>

/*
KOSVulkanWindow depends on KOSVulkanRenderer to be enabled first
KOSVulkanWindow handles everything that is presented to the window
*/
class KOSWindow {
public:


	KOSWindow(entt::registry& registry, const char* WindowTitle, int width, int height);
	bool PollEvents(); 

	const bool shouldNotClose() { return windowState; };

private:

	void CreateWin32(entt::registry& registry);

	const char* windowTitle;
	int width, height;

	bool windowState;

	HWND windowHandle;

};