#include "App.h"

void App::run()
{
	while (window.shouldNotClose())
	{

		//read for key strokes, clicks and closing the window
		window.PollEvents();
	}
	registry.clear();
}