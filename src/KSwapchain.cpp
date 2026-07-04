#include "KSwapchain.hpp"
#include "KDevice.hpp"
#include "KWindow.hpp"

namespace Kos
{
	namespace
	{

		std::vector<VkImage> arr_images;
		std::vector<VkImageView> arr_image_views;
		std::vector<VkFramebuffer> arr_frame_buffers;

		/*
		Finds a format SRGB to eliminates the need for manual gramma correction.
		Also find a color space to super the format

		In this case I use nonlinear to apply a curve to allocate more bits to the
		darker colors.
		*/
		VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR> formats)
		{
			for (const auto available_format : formats)
			{
				if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB //the format encoding
					&& available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // applys support for images in SRGB color space
				{
					return available_format;
				}

				return formats[0];
			}
		}

		/*
		*/
		VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
		{
			for (const auto& available_present_mode : presentModes)
			{
				//Single entry queue for presentation request
				if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					return available_present_mode;
				}
			}
			return VK_PRESENT_MODE_FIFO_KHR; //New request are appended to the end of the queue and the beginning one is removed
		}

		/*
		* VkExtent2D is used to define the width and hight of a image for the swapchain
		* @param
		* - VkSurfaceCapabilitiesKHR defines the capapbilities for a surface by queuing the vkGetPhysicalDeviceSurfaceCapabilitiesKHR.
		*/
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities, KWindow window)
		{
			if (capabilities.currentExtent.width != UINT32_MAX)
			{
				return capabilities.currentExtent;
			}
			else
			{
				int width, height;
				window.GetFrameBufferSize(width, height);

				VkExtent2D actual_extent =
				{
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)

				};

				actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return actual_extent;

			}
		}

		/*
		* Uses all other helpers to gather swapchain details such
		* - Surface capabilities
		* - Image formats
		* - Present modes
		*/
		Kos::SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
		{
			SwapChainSupportDetails swapchain_details;

			//Surface Capabilities
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phy_device, surface, &swapchain_details.SurfaceCapabilities);

			//Format count
			uint32_t format_count;
			vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &format_count, nullptr);

			if (format_count != 0)
			{
				swapchain_details.ImageFormats.resize(format_count);
				vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &format_count, swapchain_details.ImageFormats.data());
			}

			//Presentation modes
			uint32_t present_count;
			vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface, &present_count, nullptr);

			if (present_count != 0)
			{
				swapchain_details.PresentMode.resize(present_count);
				vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface, &present_count, swapchain_details.PresentMode.data());
			}

			return swapchain_details;
		}

	}

	void KSwapchain::CreateImageViews()
	{
		arr_image_views.resize(arr_images.size());

		for (int i = 0; i < arr_images.size(); i++)
		{
			VkImageViewCreateInfo ImageViewInfo{};
			ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ImageViewInfo.image = arr_images[i];
			ImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			ImageViewInfo.format = m_format;
			ImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			ImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			ImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			ImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			ImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			ImageViewInfo.subresourceRange.baseMipLevel = 0;
			ImageViewInfo.subresourceRange.levelCount = 1;
			ImageViewInfo.subresourceRange.baseArrayLayer = 0;
			ImageViewInfo.subresourceRange.layerCount = 1;

			VkResult result = vkCreateImageView(k_device.k_logical_device, &ImageViewInfo, nullptr, &arr_image_views[i]);

			if (result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create image views for swapchain");
			}
		}
	}

	void KSwapchain::CreateSwapchain(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
	{
		//Get SwapChain details
		SwapChainSupportDetails swapchain_details = GetSwapChainDetails(phy_device, surface);

		if (swapchain_details.ImageFormats.empty() && swapchain_details.PresentMode.empty())
			throw std::runtime_error("Failed to get images, and present modes for swapchain");

		VkSurfaceFormatKHR surface_format = ChooseSwapChainFormat(swapchain_details.ImageFormats);
		VkPresentModeKHR present_mode = ChooseSwapChainPresentMode(swapchain_details.PresentMode);
		VkExtent2D extent = ChooseSwapExtent(swapchain_details.SurfaceCapabilities, m_window);

		uint32_t image_count = swapchain_details.SurfaceCapabilities.minImageCount + 1;

		if (swapchain_details.SurfaceCapabilities.minImageCount > 0 && image_count > swapchain_details.SurfaceCapabilities.maxImageCount)
		{
			image_count = swapchain_details.SurfaceCapabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchain_Info{};
		swapchain_Info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchain_Info.surface = surface;
		swapchain_Info.minImageCount = image_count;
		swapchain_Info.imageFormat = surface_format.format;
		swapchain_Info.imageColorSpace = surface_format.colorSpace;
		swapchain_Info.imageExtent = extent;
		swapchain_Info.imageArrayLayers = 1;
		swapchain_Info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices Indices = k_device.FindQueueFamilies(phy_device, surface);
		uint32_t queueFamilyIndices[] = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };

		if (Indices.GraphicsFamily != Indices.PresentFamily)
		{
			swapchain_Info.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Image can be used by muiltple queues
			swapchain_Info.queueFamilyIndexCount = 2;
			swapchain_Info.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchain_Info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Images can only be used by a single queue
			swapchain_Info.queueFamilyIndexCount = 0;
			swapchain_Info.pQueueFamilyIndices = nullptr;
		}

		swapchain_Info.preTransform = swapchain_details.SurfaceCapabilities.currentTransform;
		swapchain_Info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // ALPHA should not be used for blending windows
		swapchain_Info.presentMode = present_mode;
		swapchain_Info.clipped = VK_TRUE; // Dont care about covered pixels
		swapchain_Info.oldSwapchain = VK_NULL_HANDLE;

		VkResult result = vkCreateSwapchainKHR(k_device.k_logical_device, &swapchain_Info, nullptr, &m_swapchain);

		if (result != VK_SUCCESS)
		{
			printf("Failed to create SwapChain");
		}

		vkGetSwapchainImagesKHR(k_device.k_logical_device, m_swapchain, &image_count, nullptr);
		arr_images.resize(image_count);
		vkGetSwapchainImagesKHR(k_device.k_logical_device, m_swapchain, &image_count, arr_images.data());

		m_format = surface_format.format;
		m_extent = extent;

		//Call Create image views

		CreateImageViews();
	}
}