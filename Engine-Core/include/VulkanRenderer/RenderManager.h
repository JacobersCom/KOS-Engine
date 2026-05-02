#include <entt/entt.hpp>

class VulkanComponents;
class VulkanSystems;

class RenderManager
{

	RenderManager(entt::registry& registry);
	~RenderManager();

	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager) = delete;

};