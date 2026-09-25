#include "KWindow.h"


_global
constexpr U16 window_h = 800;
_global
constexpr U16 window_w = 600;

void KWindow::InitalizeWindow()
{
	if (+k_window.Create(0, 0, window_w, window_h, GW::SYSTEM::GWindowStyle::WINDOWEDBORDERED))
	{
		KLog::WriteLog(LogType::Info, "Window create successfully");
		k_window.SetWindowName("KOS Engine");

		//Vulkan surface and debug layers will be pushed here and the render loop will be here
		
		Update();
	}
	else
	{
		KLog::WriteLog(LogType::Error, "Failed to create Window");
	}

}

void KWindow::Update()
{
	KLog::WriteLog(LogType::Info, "Update loop as began");

	while (true)
	{

	}
}
