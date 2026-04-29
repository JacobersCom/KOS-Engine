#include "VulkanRenderer/KOSVulkanRenderer.h"

#include <VkBootstrap.h>

#include <iostream>

KOSVulkanRenderer::KOSVulkanRenderer(std::string ApplicationName)
	:ApplicationName(ApplicationName)
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


}
