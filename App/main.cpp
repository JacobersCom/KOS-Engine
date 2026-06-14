#include "Window.hpp"
#include "RenderManager.hpp"



int main()
{
	Kos::Window window{ "KOS Engine", 600, 600 };
	RenderManager render;
	
	bool isQuit = false;
	

	window.startup();

	//add application file to just call run and put the update loop there
	while (!isQuit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{

		}
	}
	

	return EXIT_SUCCESS;
}