class basic_registry;

namespace Renderer
{
	void InitVulkanData(entt::registry& reigstry, entt::entity entity);
}

namespace Callbacks
{
	void RegistryCallback(entt::registry& registry);
}