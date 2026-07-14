#pragma once

#include "pch.hpp"

#include <vulkan/vulkan.hpp>

namespace Kos
{
		
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR SurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> ImageFormats;
		std::vector<VkPresentModeKHR> PresentMode;
	};


	class KSwapchain
	{
	public:

		KSwapchain(class KDevice* device, class KWindow* window) : m_device(device), m_window(window){};
		
		bool startup();
		bool shutdown();
		
		VkSwapchainKHR GetSwapchain() const { return m_swapchain; };
		std::vector<VkImageView> GetImageViews() const { return image_views; };
		VkExtent2D GetExtent() const { return m_extent; };
		VkFormat GetFormat() const { return m_format; };


	private:

		void CreateSwapchain(VkPhysicalDevice phy_device, VkSurfaceKHR surface, VkDevice device);

		void CreateImageViews();

	private:
		
		/*
		* Stores all the needed details of the swapchain
		*
		* Mainly used in ChooseUserGPU to ensure that the end users has the needed swapchain cabailities
		*/

		VkSwapchainKHR& m_swapchain;

		std::vector<VkImage> arr_images;
		std::vector<VkImageView> image_views;
		std::vector<VkFramebuffer> arr_frame_buffers;

		VkFormat m_format;
		VkExtent2D m_extent;

	private:

		KDevice* m_device;
		KWindow* m_window;
	};
}