#pragma once

#define GATEWARE_ENABLE_CORE
#define GATEWARE_ENABLE_SYSTEM 

#include <Gateware/Gateware.h>
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
