#include "KOSVulkanPrerequisites.h"
#include "KOSVulkanRenderer.h"

void VulkanRenderer::Init()
{
	VkApplicationInfo AppInfo{};

	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pNext = VK_NULL_HANDLE;
	AppInfo.pApplicationName = "KOS Engine";
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	AppInfo.apiVersion = VK_API_VERSION_1_4;

	VkInstance InstanceInfo{};
}
