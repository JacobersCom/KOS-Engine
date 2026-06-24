#pragma once

#include "pch.hpp"

namespace Kos
{

	/*
	* Stores all the needed details of the swapchain
	*
	* Mainly used in ChooseUserGPU to ensure that the end users has the needed swapchain cabailities
	*/
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR SurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> ImageFormats;
		std::vector<VkPresentModeKHR> PresentMode;
	};

	class KSwapchain
	{

	};
}