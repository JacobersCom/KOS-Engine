#pragma once

#include "pch.hpp"

#include <vulkan/vulkan.hpp>

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
	public:

		KSwapchain();

		void CreateSwapchain();
		SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice phy_device, VkSurfaceKHR surface);

		std::vector<VkImage> _VkSwapchainImages;
		std::vector<VkImageView> _VkSwapchainImageViews;
		std::vector<VkFramebuffer> _VkFramebuffers;
	};
}