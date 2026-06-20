
#include "KWindow.hpp"
#include "KRender.hpp"
#include "KVector3.hpp"
#include "KMat4.hpp"

#include <iostream>

int main()
{
	Kos::KWindow _win;
	Kos::KRender _render(&_win);

	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);
	_win.Create();
	
	bool IsRunning = _render.InitVulkan();

	while (IsRunning)
	{
		_render.UpdateLoop();
		
		if (!IsWindow(_win.GetWindowHandle()))
		{
			IsRunning = false;
		}
	}


}