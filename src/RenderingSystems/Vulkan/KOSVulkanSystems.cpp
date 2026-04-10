#include "RenderingSystems/Vulkan/KOSVulkanComponents.h"

/*
Finds the graphics and presentation queue familys for rendering
The data for this function is held within KOSVulkanComponents
*/
QueueFamilys FindQueueFamilys(VkPhysicalDevice PhysicalDevice)
{
	//Enables the code to access the original queue family to avoid having to make copies later
	std::unique_ptr<QueueFamilys> indices = std::make_unique<QueueFamilys>();
	std::unique_ptr<Surfaces> surf = std::make_unique<Surfaces>();

	uint32_t count;
	VkBool32 PresentationEnabled;

	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &count, nullptr);

	std::vector<VkQueueFamilyProperties> Familys(count);

	vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &count, Familys.data());


	for (uint32_t i = 0; i < Familys.size(); i++)
	{

		if (Familys[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices->GraphicsBit = i;
		}

		vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, i, surf->Win32Surface, &PresentationEnabled);

		if (PresentationEnabled)
		{
			indices->PresentBit = i;
		}
		break;
	}
}