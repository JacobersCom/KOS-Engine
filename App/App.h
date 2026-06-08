#pragma once

#include "VulkanRenderer/KOSWindow.h"
#include "VulkanRenderer/RenderManager.h"


class App
{
public:

	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;


	void run();

private:
	
	//KOSWindow resource initialization
	RenderManager renderer{};
	ENGINE::KOSWindow window{"TestWindow", 800, 800 };
};