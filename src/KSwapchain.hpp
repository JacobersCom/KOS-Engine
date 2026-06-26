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

		KSwapchain(class KDevice& device, class KWindow& window) : m_device(device), m_window(window){};

		
		void CreateSwapchain(VkPhysicalDevice phy_device, VkSurfaceKHR surface);

		//Helper functions
		SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice phy_device, VkSurfaceKHR surface);

		std::vector<VkImage> _VkSwapchainImages;
		std::vector<VkImageView> _VkSwapchainImageViews;
		std::vector<VkFramebuffer> _VkFramebuffers;

	private:

		KDevice& m_device;
		KWindow& m_window;
	};
}