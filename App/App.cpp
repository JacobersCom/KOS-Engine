#include "App.h"

void App::run()
{
	while (!window.shouldClose())
	{
		//read for key strokes, clicks and closing the window
		window.PollEvents();
	}
}