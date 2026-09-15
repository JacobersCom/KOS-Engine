#include "KRenderer.h"

#include "KLog.h"


bool KRenderer::InitRenderer()
{
	//Applcation information
	VkApplicationInfo app_info{};


	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = VK_NULL_HANDLE;
	app_info.apiVersion = VK_API_VERSION_1_4; 

	VkInstanceCreateInfo instance_info{};

	std::vector<const char*> instance_exts = GetRequiredInstanceExtensions();

	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;
	instance_info.pNext = VK_NULL_HANDLE;
	instance_info.enabledExtensionCount = static_cast<uint32_t>(instance_exts.size());
	instance_info.ppEnabledExtensionNames = instance_exts.data();

	if (enableValidationLayers && !CheckVaildationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available");
	}
	else
	{
		_VkValidationLayers = GetRequiredInstaceLayers();
		instance_info.enabledLayerCount = static_cast<uint32_t>(_VkValidationLayers.size());
		instance_info.ppEnabledLayerNames = _VkValidationLayers.data();
	}


	KLog::VulkanLog(vkCreateInstance(&instance_info, nullptr, &k_instance);
}
