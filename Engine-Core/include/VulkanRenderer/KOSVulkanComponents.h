#include <vulkan/vulkan.h>
#include <VkBootstrap.h>

namespace Renderer
{
	//Everything to init vulkan
	struct VulkanData
	{
		vkb::Instance vkbIns;
		vkb::PhysicalDevice vkbPhy;
		vkb::Device vkbDev;
		vkb::Swapchain vkbSwapchain;
	};

	struct QueueFamilys
	{
		uint32_t GraphicsBit;
		uint32_t PresentBit;
	};

	struct Surfaces
	{
		VkSurfaceKHR Win32Surface;
	};

}

