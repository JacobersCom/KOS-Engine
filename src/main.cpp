
#include "RHI/Vulkan/KRender.h"
#include "Core/Window/KWindow.h"
#include <iostream>
#include <Gateware.h>


int main()
{
	KE::SYSTEM::KWindow _win;
	KE::RENDERER::KRender _render(&_win);

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

	_render.CleanUp();




	
}