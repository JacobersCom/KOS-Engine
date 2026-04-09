#include "KOSVulkanRenderer.h"


void VulkanRenderer::CreateInstance()
{
	VkApplicationInfo AppInfo{};

	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pNext = VK_NULL_HANDLE;
	AppInfo.pApplicationName = "KOS Engine";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.apiVersion = VK_API_VERSION_1_4;

	VkInstanceCreateInfo InstanceInfo{};
	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pApplicationInfo = &AppInfo;

#if _DEBUG
	auto& Layers = GetRequiredDebuggingLayers();

	InstanceInfo.enabledLayerCount = Layers.size();
	InstanceInfo.ppEnabledLayerNames = Layers.data();
#endif

	//Replace when logger is created
	try {
		VkResult result = vkCreateInstance(&InstanceInfo, nullptr, &KInstance);
		if (result == VK_SUCCESS)
		{
			printf("- Vulkan Instance Created");
		}
		throw result;
	}
	catch (int errorCode)
	{
		printf("- Vulkan Instance Creation Failed");
	}
}

std::vector<const char*> VulkanRenderer::GetRequiredDebuggingLayers()
{
	uint32_t count;

	vkEnumerateInstanceLayerProperties(&count, nullptr);
	
	std::vector<VkLayerProperties> AvailableLayers(count);
	vkEnumerateInstanceLayerProperties(&count, AvailableLayers.data());
	
	std::vector<const char*> WantedLayers = { "VK_LAYER_KHRONOS_validation" };

	for (const auto& Layers : WantedLayers)
	{
		bool found = false;

		for (const auto& AllowedLayers : AvailableLayers)
		{
			if (strcmp(Layers, AllowedLayers.layerName) == 0)
			{
				found = true;
				break;
			}
		}
	}
	return WantedLayers;
	
}
