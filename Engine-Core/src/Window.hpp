#pragma once

#include "SDL3/SDL.h"

namespace Kos
{
	class Window {
	public:

		Window(const char*  WindowTitle, int width, int height);

		bool startup();
		void shutdown();

	private:
		
		const char* windowTitle;
		int width, height;
		
		SDL_Window* windowHandle = nullptr;
	};
}
