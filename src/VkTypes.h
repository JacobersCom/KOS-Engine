#pragma once

#include "VkCommon.h"

internal
struct SwapChainDetails
{
	VkSurfaceCapabilitiesKHR		surface_capabilities;
	std::vector<VkSurfaceFormatKHR> image_format;
	std::vector<VkPresentModeKHR>	present_mode;
};

internal
struct QueueFamilys
{
	std::optional<U32> graphics_id;

	bool HasQueueFamilys()
	{
		return graphics_id.has_value();
	}
};

internal
struct Image
{
	VkImage		image_handle;
	VkImageView image_data;
	VkExtent2D  image_extent;
};