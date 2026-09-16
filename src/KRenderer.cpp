#include "KRenderer.h"

#include "VkUtils.h"

#include "KLog.h"


bool KRenderer::CreateInstance(const char* app_name, U32 verison)
{
	//Applcation information
	VkApplicationInfo app_info{};


	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = VK_NULL_HANDLE;
	app_info.pApplicationName = app_name;
	app_info.applicationVersion = verison;
	app_info.apiVersion = VK_API_VERSION_1_4; 

	VkInstanceCreateInfo instance_info{};

	std::vector<const char*> instance_exts = VkUtils::GetRequiredInstanceExtensions();

	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;
	instance_info.pNext = VK_NULL_HANDLE;
	instance_info.enabledExtensionCount = static_cast<uint32_t>(instance_exts.size());
	instance_info.ppEnabledExtensionNames = instance_exts.data();

	if (enableValidationLayers && !VkUtils::CheckVaildationLayerSupport(instance_exts))
	{
		KLog::WriteLog(LogType::VError,"Requested validation layers with out vaildation layer support");
		return false;
	}
		
	instance_info.enabledLayerCount = static_cast<uint32_t>(instance_exts.size());
	instance_info.ppEnabledLayerNames = instance_exts.data();
	

	KLog::VulkanCheck(vkCreateInstance(&instance_info, nullptr, &k_instance));
}
