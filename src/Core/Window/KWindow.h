#pragma once

#include <Common.h>

class KWindow
{
public:

	void InitalizeWindow();
	void Update();

private:


	GW::SYSTEM::GWindow		    k_window;
	GW::SYSTEM::GWindow::Events k_window_event;
};
