#include "KSwapchain.hpp"
#include "KDevice.hpp"

namespace Kos
{
	namespace
	{
		/*
	*/
		Kos::SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
		{
			SwapChainSupportDetails SwapChainDetails;

			//Surface Capabilities
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phy_device, surface, &SwapChainDetails.SurfaceCapabilities);

			//Format count
			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &formatCount, nullptr);

			if (formatCount != 0)
			{
				SwapChainDetails.ImageFormats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &formatCount, SwapChainDetails.ImageFormats.data());
			}

			//Presentation modes
			uint32_t presentCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface, &presentCount, nullptr);

			if (presentCount != 0)
			{
				SwapChainDetails.PresentMode.resize(presentCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface, &presentCount, SwapChainDetails.PresentMode.data());
			}

			return SwapChainDetails;
		}

		/*
		Finds a format SRGB to eliminates the need for manual gramma correction.
		Also find a color space to super the format

		In this case I use nonlinear to apply a curve to allocate more bits to the
		darker colors.
		*/
		VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR> formats)
		{
			for (const auto availableFormat : formats)
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB //the format encoding
					&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // applys support for images in SRGB color space
				{
					return availableFormat;
				}

				return formats[0];
			}
		}

		/*
		*/
		VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
		{
			for (const auto& availablePresentMode : presentModes)
			{
				//Single entry queue for presentation request
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					return availablePresentMode;
				}
			}
			return VK_PRESENT_MODE_FIFO_KHR; //New request are appended to the end of the queue and the beginning one is removed
		}

		/*
		* VkExtent2D is used to define the width and hight of a image for the swapchain
		* @param
		* - VkSurfaceCapabilitiesKHR defines the capapbilities for a surface by queuing the vkGetPhysicalDeviceSurfaceCapabilitiesKHR.
		*/
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities)
		{
			if (capabilities.currentExtent.width != UINT32_MAX)
			{
				return capabilities.currentExtent;
			}
			else
			{
				int width, height;
				_win->GetFrameBufferSize(_win->GetWindowHandle(), width, height);

				VkExtent2D actualExtent =
				{
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)

				};

				actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return actualExtent;

			}
		}
	}

	Kos::SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails SwapChainDetails;

		//Surface Capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phy_device, surface, &SwapChainDetails.SurfaceCapabilities);

		//Format count
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			SwapChainDetails.ImageFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &formatCount, SwapChainDetails.ImageFormats.data());
		}

		//Presentation modes
		uint32_t presentCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface, &presentCount, nullptr);

		if (presentCount != 0)
		{
			SwapChainDetails.PresentMode.resize(presentCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface, &presentCount, SwapChainDetails.PresentMode.data());
		}

		return SwapChainDetails;
	}

	void Kos::KSwapchain::CreateSwapchain(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
	{
		//Get SwapChain details
		SwapChainSupportDetails SwapChainDetails = GetSwapChainDetails(phy_device);

		VkSurfaceFormatKHR SurfaceFormat = ChooseSwapChainFormat(SwapChainDetails.ImageFormats);
		VkPresentModeKHR PresentMode = ChooseSwapChainPresentMode(SwapChainDetails.PresentMode);
		VkExtent2D Extent = ChooseSwapExtent(SwapChainDetails.SurfaceCapabilities);

		uint32_t ImageCount = SwapChainDetails.SurfaceCapabilities.minImageCount + 1;

		if (SwapChainDetails.SurfaceCapabilities.minImageCount > 0 && ImageCount > SwapChainDetails.SurfaceCapabilities.maxImageCount)
		{
			ImageCount = SwapChainDetails.SurfaceCapabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR SwapChainInfo{};
		SwapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		SwapChainInfo.surface = surface;
		SwapChainInfo.minImageCount = ImageCount;
		SwapChainInfo.imageFormat = SurfaceFormat.format;
		SwapChainInfo.imageColorSpace = SurfaceFormat.colorSpace;
		SwapChainInfo.imageExtent = Extent;
		SwapChainInfo.imageArrayLayers = 1;
		SwapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices Indices = FindQueueFamilies(phy_device);
		uint32_t queueFamilyIndices[] = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };

		if (Indices.GraphicsFamily != Indices.PresentFamily)
		{
			SwapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Image can be used by muiltple queues
			SwapChainInfo.queueFamilyIndexCount = 2;
			SwapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			SwapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Images can only be used by a single queue
			SwapChainInfo.queueFamilyIndexCount = 0;
			SwapChainInfo.pQueueFamilyIndices = nullptr;
		}

		SwapChainInfo.preTransform = SwapChainDetails.SurfaceCapabilities.currentTransform;
		SwapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // ALPHA should not be used for blending windows
		SwapChainInfo.presentMode = PresentMode;
		SwapChainInfo.clipped = VK_TRUE; // Dont care about covered pixels
		SwapChainInfo.oldSwapchain = VK_NULL_HANDLE;

		VkResult result = vkCreateSwapchainKHR(_VkDevice, &SwapChainInfo, nullptr, &_VkSwapchain);

		if (result != VK_SUCCESS)
		{
			printf("Failed to create SwapChain");
		}

		vkGetSwapchainImagesKHR(_VkDevice, _VkSwapchain, &ImageCount, nullptr);
		_VkSwapchainImages.resize(ImageCount);
		vkGetSwapchainImagesKHR(_VkDevice, _VkSwapchain, &ImageCount, _VkSwapchainImages.data());

		_VkSwapchainFormat = SurfaceFormat.format;
		_VkSwapchainExtent = Extent;
	}
}