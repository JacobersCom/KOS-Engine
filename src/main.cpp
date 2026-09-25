
#include "Core/Window/KWindow.h"


int main()
{
	KWindow win;

	win.InitalizeWindow("KOS-Engine", 800, 800);

	while (win.IsWindowOpen())
	{
		win.Update();
	}

}

