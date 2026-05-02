#include "VulkanRenderer/RenderManager.h"
#include "VulkanRenderer/KOSVulkanComponents.h"


#include <entt/entt.hpp>
#include "VkBootstrap.h"

#include <iostream>

namespace Renderer
{

	void InitVulkanData(entt::registry& reigstry, entt::entity entity)
	{
		vkb::InstanceBuilder builder;
		vkb::Result<vkb::Instance> instResults = builder.set_app_name("KOS-Engine")
			.request_validation_layers()
			.use_default_debug_messenger()
			.build();
		if (!instResults)
		{
			std::cout << instResults.error().message() << "\n";
			EXIT_FAILURE;
		}
		auto& vkbIns = reigstry.try_get<Renderer::VulkanData>(entity)->vkbIns;
		if (nullptr != vkbIns)
		{
			vkbIns = instResults.value();
		}
	}

}

namespace Callbacks
{
	void RegistryCallback(entt::registry& registry)
	{
		registry.on_construct<Renderer::VulkanData>().connect<>
	}
}

