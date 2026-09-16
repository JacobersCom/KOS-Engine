#pragma once

#include "VkCommon.h"

#ifdef NDEBUG
static const bool enableValidationLayers = false;
#else 
static const bool enableValidationLayers = true;
#endif

class VkUtils
{
public:

	static inline 
	std::vector<const char*> GetRequiredInstanceExtensions()
	{
		std::vector<const char*> extensions;

		if (enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		return extensions;
	}

	static inline
	bool CheckVaildationLayerSupport(std::vector<const char*> validation_layers)
	{
		uint32_t LayerCount;
		vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

		std::vector<VkLayerProperties> available_layers(LayerCount);
		vkEnumerateInstanceLayerProperties(&LayerCount, available_layers.data());

		for (const auto* layer_name : validation_layers)
		{
			bool Layer_found = false;

			for (const auto& layer_properties : available_layers)
			{
				if (strcmp(layer_name, layer_properties.layerName) == 0)
				{
					Layer_found = true;
					break;
				}
			}

			if (!Layer_found)
				return false;
		}
		return true;
	}

private:

};