
#include "KRender.h"
#include "KWindow.h"
#include <iostream>

#include "KLog.h"

int main()
{
	KE::SYSTEM::KWindow _win;
	KE::RENDERER::KRender _render(&_win);

	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);
	_win.Create();
	
	KLog::WriteLog(LogType::Error, "Working");

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