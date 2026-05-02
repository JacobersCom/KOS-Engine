#pragma once


#include <entt/entt.hpp>

class RenderManager
{
public:

	RenderManager(entt::registry& registry);
	//~RenderManager();

	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager) = delete;

};