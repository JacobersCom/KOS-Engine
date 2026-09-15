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
};