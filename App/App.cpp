#include "App.h"

//Possibly make run on a different thread
void App::run()
{
	while (window.shouldClose())
	{

		//read for key strokes, clicks and closing the window
		window.PollEvents();
	}
	registry.clear();
}