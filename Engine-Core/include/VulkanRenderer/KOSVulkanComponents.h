#include <vulkan/vulkan.h>

#include <VkBootstrap.h>


namespace DRAW
{

	struct VulkanRenderer
	{
		vkb::Instance vkbIns;
		vkb::PhysicalDevice vkbPhy;
		vkb::Device vkbDev;
		vkb::Swapchain vkbSwapChain;
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

