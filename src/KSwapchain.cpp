#include "KSwapchain.h"
#include "KWindow.h"

Kos::KSwapchain::KSwapchain()
{
}

Kos::KSwapchain::~KSwapchain()
{

}


bool Kos::KSwapchain::GetSwapchainDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
{

	//Get the capabilities of the users surfaces
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.SurfaceCapabilities);

	//Query color formats supported by surface
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount == 0)
	{
		return false;
	}
	details.ImageFormats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.ImageFormats.data());

	uint32_t presentCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, nullptr);

	if (presentCount == 0)
	{
		return false;
	}
	details.PresentMode.resize(presentCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, details.PresentMode.data());

	return true;

}

VkSurfaceFormatKHR Kos::KSwapchain::ChooseSwapchainFormat(const std::vector<VkSurfaceFormatKHR> formats)
{
	for (const auto availableFormat : formats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
			&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // applys support for images in SRGB color space
		{
			return availableFormat;
		}
		return formats[0];
	}
}


VkPresentModeKHR Kos::KSwapchain::ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> present)
{
	for (const auto& availablePresentMode : details.PresentMode)
	{
		//Single entry queue for presentation request
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR; //New request are appended to the end of the queue and the beginning one is removed
}


VkExtent2D Kos::KSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR capabilities)
{
	//Allocate memory to window pointer
	window = std::make_unique<KWindow>();

	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		//Gets the frame buffer size and stores in within width and height
		int width, height;
		window->GetFrameBufferSize(window->GetWindowHandle(), width, height);

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

