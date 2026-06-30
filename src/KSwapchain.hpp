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

		KSwapchain(class KDevice& device, class KWindow& window) : m_Kdevice(device), m_window(window){};

		
		void CreateSwapchain(VkPhysicalDevice phy_device, VkSurfaceKHR surface);


		VkSwapchainKHR m_swapchain;

		std::vector<VkImage> m_images_arr;
		std::vector<VkImageView> image_views_arr;
		std::vector<VkFramebuffer> frame_buffers_arr;

		VkFormat m_format;
		VkExtent2D m_extent;

	private:

		KDevice& m_Kdevice;
		KWindow& m_window;
	};
}