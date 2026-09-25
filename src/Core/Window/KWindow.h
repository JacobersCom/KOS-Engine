#pragma once

#include "../Utils/Common.h"

class KWindow
{
public:

	void InitalizeWindow(const char* window_name, int width, int height);
	
	bool IsWindowOpen() const { return is_open; }

	void Update();

private:

	bool						 is_open;
	GW::SYSTEM::GWindow		     k_window;
};
