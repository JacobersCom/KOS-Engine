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

		KSwapchain(class KDevice& device, class KWindow& window) : k_device(device), m_window(window){};
		
		bool startup(VkPhysicalDevice phy_device, VkSurfaceKHR surface);
		bool shutdown();
		
		void CreateSwapchain(VkPhysicalDevice phy_device, VkSurfaceKHR surface);

		void CreateImageViews();


		VkSwapchainKHR m_swapchain;

		std::vector<VkImage> arr_images;
		std::vector<VkImageView> arr_image_views;
		std::vector<VkFramebuffer> arr_frame_buffers;

		VkFormat m_format;
		VkExtent2D m_extent;

	private:

		KDevice& k_device;
		KWindow& m_window;
	};
}