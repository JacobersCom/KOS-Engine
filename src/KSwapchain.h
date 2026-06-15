#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <algorithm>
#include <iostream>

namespace Kos
{
	class KSwapchain
	{
		KSwapchain();
		~KSwapchain();

		bool CreateSwapchain(VkPhysicalDevice);

		/*
		Gets all the physical devices information need for the swapchain.
		Such as surface capabilities, format and present modes
		@param
		- VkPhysical - handle to the users physical device
		@param
		- VkSurfaceKHR - handle to the window surface
		*/
		bool GetSwapchainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
		
		/*
		Chooses the supported image format for the swapchain
		@param
		- Vector of VkSurfaceFormatKHR
		*/
		VkSurfaceFormatKHR ChooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR> formats);
		
		/*
		Chooses the supported present modes for the swapchain
		@param
		- Vector of VkPresentModeKHR
		*/
		VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> present);
		
		/*
		Choose the Swapchain extent which is the size of the images vulkan will present
		*/
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR capabilities);

	private:

		struct SwapchainDetails
		{
			VkSurfaceCapabilitiesKHR SurfaceCapabilities;
			std::vector<VkSurfaceFormatKHR> ImageFormats;
			std::vector<VkPresentModeKHR> PresentMode;
		};

		SwapchainDetails details;

		std::unique_ptr<class KWindow> window;

		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;
	};
}
