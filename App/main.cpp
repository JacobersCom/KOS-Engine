#include "KOSWindow.hpp"
#include "RenderManager.hpp"

#include <iostream>

int main()
{
	ENGINE::KOSWindow window{ "KOS Engine", 600, 600 };
	RenderManager render;


	try
	{
		window.startup();
	}
	catch(const std::exception& error)
	{
		std::cerr << error.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}