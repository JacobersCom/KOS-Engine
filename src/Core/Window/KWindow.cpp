#include "KWindow.h"


void KWindow::InitalizeWindow(const char* window_name, int width, int height)
{
	if (-k_window.Create(0, 0, width, height, GW::SYSTEM::GWindowStyle::WINDOWEDBORDERED))
	{
		KLog::WriteLog(LogType::Error, "Failed to create Window");
		abort();
	}
	KLog::WriteLog(LogType::Info, "Window create successfully");
	
	k_window.SetWindowName(window_name);

	is_open = true;
}

void KWindow::Update()
{
	KLog::WriteLog(LogType::Info, "Update loop as began");

	while (+k_window.ProcessWindowEvents())
	{
		//Render loop

	}

	is_open = false;
}
