#pragma once

#include "../Utils/Common.h"

class KWindow
{
public:

	KWindow(const char* window_name, U32 width, U32 height)
		: window_name(window_name), width(width), height(height) {

		is_open = false;
	};

	void InitalizeWindow();
	
	bool IsWindowOpen() const { return is_open; }

	void Update();

private:

	U32 width, height;
	const char* window_name;

	bool						 is_open;
	GW::SYSTEM::GWindow		     k_window;
};
