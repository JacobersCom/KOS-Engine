#include "VulkanRenderer/RenderManager.h"
#include "VulkanRenderer/KOSVulkanComponents.h"

//Replce this call with the function body
inline void RegisterCallback(entt::registry& registry);

RenderManager::RenderManager(entt::registry& registry)
{

	/* Creates a new thread that creates the render manager
	and ensures registry is a & */
	RenderThread = std::thread(&RenderManager::StartRenderThread, this,std::ref(registry));
}

void RenderManager::StartRenderThread(entt::registry& registry)
{
	RegisterCallback(registry);

	entt::entity Manager = registry.create();

	registry.emplace<RENDERER::Surface>(Manager);
	registry.emplace<RENDERER::VulkanData>(Manager);

}