#include "VulkanRenderer/RenderManager.h"
#include "VulkanRenderer/KOSVulkanComponents.h"


RenderManager::RenderManager(entt::registry& registry)
{
	//Create all systems for rendering


	entt::entity Manager = registry.create();

	registry.emplace<RENDERER::Surface>(Manager);
	registry.emplace<RENDERER::VulkanData>(Manager);
}