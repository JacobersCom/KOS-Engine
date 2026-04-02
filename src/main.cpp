
#include <platform/win32/KWindow.h>
#include <render/vulkan/KRender.h>
#include "KVector3.h"
#include "KMat4.h"

#include <iostream>

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
		_win.EventDispatcher();
		
		if (!IsWindow(_win.GetWindowHandle()))
		{
			IsRunning = false;
		}
	}

	_render.CleanUp();




	
}