#include "VulkanRenderer/RenderManager.h"
#include "VulkanRenderer/KOSVulkanComponents.h"
#include "VulkanRenderer/KOSWindow.h"

#include "VkBootstrap.h"

#include <iostream>


namespace RENDERER
{

	void Construct_VulkanData(entt::registry& registry, entt::entity entity)
	{
		vkb::InstanceBuilder builder;
		auto instResults = builder.set_app_name("KOS-Engine")
			.request_validation_layers()
			.use_default_debug_messenger()
			.build();
		if (!instResults)
		{
			std::cout << instResults.error().message() << "\n";
			EXIT_FAILURE;
		}
		auto& vkbIns = registry.try_get<RENDERER::VulkanData>(entity)->vkbIns;
		vkbIns = instResults.value();
		

		vkb::PhysicalDeviceSelector selector{ vkbIns };
		auto& vkbSur = registry.try_get<RENDERER::Surface>(entity)->Win32Surface;
		vkb::Result<vkb::PhysicalDevice> phyResults = selector.set_surface(vkbSur)
			.set_minimum_version(1, 1)
			.require_dedicated_transfer_queue()
			.select();
		if (!phyResults)
		{
			std::cout << phyResults.error().message() << "\n";
			EXIT_FAILURE;
		}
		auto& vkbPhy = registry.try_get<RENDERER::VulkanData>(entity)->vkbPhy;
		vkbPhy = phyResults.value();

	}

	void Construct_Surface(entt::registry& registry, entt::entity entity)
	{
		auto* Surface = registry.try_get<RENDERER::Surface>(entity);

		if (Surface == nullptr)
		{
			printf("Error: %s\n", strerror(errno));
			EXIT_FAILURE;
		}

		VkWin32SurfaceCreateInfoKHR Win32SurInfo{};
		Win32SurInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		Win32SurInfo.hinstance = GetModuleHandleA(nullptr);
		Win32SurInfo.hwnd = Surface->WindowHandle;
	
	}

}


void RegisterCallback(entt::registry& registry)
{
	registry.on_construct<RENDERER::Surface>().connect<&RENDERER::Construct_Surface>();
	registry.on_construct<RENDERER::VulkanData>().connect<&RENDERER::Construct_VulkanData>();

}


