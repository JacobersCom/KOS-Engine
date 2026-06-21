#include "KDevice.hpp"

void Kos::KDevice::CreateInstance()
{
	VkApplicationInfo AppInfo{};


	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pNext = VK_NULL_HANDLE;
	AppInfo.pApplicationName = "KOS Engine"; //Name of the applcation
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); //Version of the applcation
	AppInfo.pEngineName = "KOS"; //Name of engine used to make the applcation
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); //Version of the engine used to make the applcation
	AppInfo.apiVersion = VK_API_VERSION_1_4; //Version of the vulkan instance

	//The parameters for the newly created vulkan instance
	VkInstanceCreateInfo InstanceInfo{};

	std::vector<const char*> layers =
	{
		"VK_LAYER_KHRONOS_validation"
		"VK_EXT_debug_utils"
	};

	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pApplicationInfo = &AppInfo;
	InstanceInfo.pNext = VK_NULL_HANDLE;
	InstanceInfo.enabledExtensionCount = 0;
	InstanceInfo.ppEnabledExtensionNames = nullptr;

#ifndef _DEBUG
	if (CheckVaildationLayersSupport(layers))
	{
		InstanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		InstanceInfo.ppEnabledLayerNames = layers.data();
	}
#endif // !NDEBUG

	if (vkCreateInstance(&InstanceInfo, nullptr, &mInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create VkInstance");
	}
}

bool Kos::KDevice::CheckVaildationLayersSupport(std::vector<const char*> layers)
{
	uint32_t LayerCount;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, availableLayers.data());

	for (const auto* LayerName : layers)
	{
		bool LayerFound = false;

		for (const auto& LayerProperties : availableLayers)
		{
			if (strcmp(LayerName, LayerProperties.layerName) == 0)
			{
				LayerFound = true;
				break;
			}
		}

		if (!LayerFound)
			return false;
	}
	return true;
}