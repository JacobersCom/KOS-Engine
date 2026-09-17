#pragma once

#include "VkCommon.h"
#include "VkTypes.h"


#ifdef NDEBUG
global const bool enable_validation_layers = false;
#else 
global const bool enable_validation_layers = true;
#endif

class VkUtils
{
public:

	internal inline
	bool IsDeviceSuitable(VkPhysicalDevice physical_device)
	{
		QueueFamilyIndices Indices = KRender::FindQueueFamilies(_VkPhyscialDevice);

		bool extensionsSupported = CheckDeviceExtensionSupport(_VkPhyscialDevice);

		//Is the SwapChain supported
		bool SwapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails SwapChainSupportDetails = GetSwapChainDetails(physical_device);
			SwapChainAdequate = !SwapChainSupportDetails.ImageFormats.empty() && !SwapChainSupportDetails.PresentMode.empty();
		}

		return Indices.isComplete() && SwapChainAdequate && extensionsSupported;
	}

	internal inline 
	std::vector<const char*> GetRequiredInstanceExtensions()
	{
		std::vector<const char*> extensions;

		if (enable_validation_layers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		if (windows)
		{
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		}

		return extensions;
	}

	internal inline
	bool CheckVaildationLayerSupport(std::vector<const char*> validation_layers)
	{
		if (!enable_validation_layers)
			return false;

		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> available_layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

		for (const auto* layer_name : validation_layers)
		{
			bool layer_found = false;

			for (const auto& layer_properties : available_layers)
			{
				if (strcmp(layer_name, layer_properties.layerName) == 0)
				{
					layer_found = true;
					break;
				}
			}

			if (!layer_found)
				return false;
		}
		return true;
	}

private:

};