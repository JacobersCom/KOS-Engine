#include "VulkanRenderer/RenderManager.h"
#include "VulkanRenderer/KOSVulkanComponents.h"
#include "VulkanRenderer/KOSWindow.h"

#include "VkBootstrap.h"

#include <chrono>
#include <iostream>




void Construct_VulkanData(entt::registry& registry, entt::entity entity)
{
	auto* sync = registry.try_get<RENDERER::RenderSync>(entity);

	//Locks thread

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



	//Tell the render thread to wait until the surface is ready

	
	vkb::PhysicalDeviceSelector selector{ vkbIns };
	auto& displayView = registry.view<RENDERER::Surface>();
	
	entt::entity display = displayView.begin()[0];

	auto& Win32Surface = registry.try_get<RENDERER::Surface>(display)->Win32Surface;
	
	vkb::Result<vkb::PhysicalDevice> phyResults = selector.set_surface(Win32Surface)
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
	auto* sync = registry.try_get<RENDERER::RenderSync>(entity);



	if (Surface == nullptr)
	{
		printf("Error: %s\n", strerror(errno));
		EXIT_FAILURE;
	}

	VkWin32SurfaceCreateInfoKHR Win32SurInfo{};
	Win32SurInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	Win32SurInfo.hinstance = GetModuleHandleA(nullptr);
	Win32SurInfo.hwnd = Surface->WindowHandle;

	auto& VkDataView = registry.view<RENDERER::VulkanData>();

	entt::entity renderManager = VkDataView.begin()[0];

	auto& vkbIns = registry.try_get<RENDERER::VulkanData>(renderManager)->vkbIns;


	//Lock Main thread here and tell it to wait until the ins is ready

	auto result = vkCreateWin32SurfaceKHR(vkbIns, &Win32SurInfo, nullptr, &Surface->Win32Surface);

	if (result != VK_SUCCESS)
	{
		printf("Error: %s\n" "Failed to create window surface");
		EXIT_FAILURE;
	}

	

	//Let render thread know the surface is ready
	

}


void RegisterCallback(entt::registry& registry)
{
	registry.on_construct<RENDERER::VulkanData>().connect<Construct_VulkanData>();
	registry.on_construct<RENDERER::Surface>().connect<Construct_Surface>();

}


