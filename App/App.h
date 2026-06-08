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
	entt::registry registry;
	
	//KOSWindow resource initialization
	RenderManager renderer{ registry };
	ENGINE::KOSWindow window{ registry, "TestWindow", 800, 800 };
};