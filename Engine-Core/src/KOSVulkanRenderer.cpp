#include "VulkanRenderer/KOSVulkanRenderer.h"

#include <VkBootstrap.h>
#include <entt/entt.hpp>

#include <iostream>

KOSVulkanRenderer::KOSVulkanRenderer(std::string AppName)
	:ApplicationName(AppName)
{
	InitVulkan();
}

void KOSVulkanRenderer::InitVulkan()
{
	vkb::InstanceBuilder InsBuilder;

	vkb::Result<vkb::Instance> InsReturn = InsBuilder.set_app_name(ApplicationName.c_str())
		.request_validation_layers()
		.use_default_debug_messenger()
		.build();

	if (!InsReturn)
	{
		std::cout << InsReturn.error().message() << "\n";
		
		return;
	}
	vkb_Ins = InsReturn.value();

	vkb::PhysicalDeviceSelector PhySelector(vkb_Ins);
	vkb::Result<vkb::PhysicalDevice> PhyReturn = PhySelector.set_surface()





}
