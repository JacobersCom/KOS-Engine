#pragma once


#include <entt/entt.hpp>
#include <thread>

class RenderManager
{
public:

	RenderManager(entt::registry& registry);
	//~RenderManager();

	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager) = delete;


private:

	void CreateManager(entt::registry& registry);

	std::thread RenderThread;
};