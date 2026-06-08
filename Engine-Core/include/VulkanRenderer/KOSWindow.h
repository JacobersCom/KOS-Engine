#pragma once

#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#include "Windows.h"
#endif


//deps

/*
KOSVulkanWindow depends on KOSVulkanRenderer to be enabled first
KOSVulkanWindow handles everything that is presented to the window
*/
namespace ENGINE
{

	class KOSWindow {
	public:


		KOSWindow(const char* WindowTitle, int width, int height);
		void startup();

		const bool shouldNotClose() { return windowState; };

	private:

		bool PollEvents();

		const char* windowTitle;
		int width, height;

		bool windowState = false;


	};
}
