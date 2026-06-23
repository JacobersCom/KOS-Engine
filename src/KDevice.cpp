#include "KDevice.hpp"
#include "KWindow.hpp"

namespace
{
	/*
	Checks the end users GPU for
		- swapchain support
		- Graphics Queue
		- Present Queue
	*/
	bool IsDeviceSuitable(VkPhysicalDevice _VkPhyscialDevice)
	{
		Kos::QueueFamilyIndices Indices = FindQueueFamilies(_VkPhyscialDevice);

		bool extensionsSupported = CheckDeviceExtensionSupport(_VkPhyscialDevice);

		//Is the SwapChain supported
		bool SwapChainAdequate = false;
		if (extensionsSupported)
		{
			Kos::SwapChainSupportDetails SwapChainSupportDetails = GetSwapChainDetails(_VkPhyscialDevice);
			SwapChainAdequate = !SwapChainSupportDetails.ImageFormats.empty() && !SwapChainSupportDetails.PresentMode.empty();
		}

		return Indices.isComplete() && SwapChainAdequate && extensionsSupported;
	}

	/*
	 Ensures the end users has the support vaildation layers
	 
	 The current layers used by KOS is
			- "VK_LAYER_KHRONOS_validation"
			- "VK_EXT_debug_utils"
	*/
	bool CheckVaildationLayersSupport(std::vector<const char*> layers)
	{
		uint32_t LayerCount;
		vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(LayerCount);
		vkEnumerateInstanceLayerProperties(&LayerCount, availableLayers.data());

		for (const auto* LayerName : layers)
		{
			bool LayerFound = false;

			for (const auto& LayerProperties : availableLayers)
			{
				if (strcmp(LayerName, LayerProperties.layerName) == 0)
				{
					LayerFound = true;
					break;
				}
			}

			if (!LayerFound)
				return false;
		}
		return true;
	}
}

/*
* Creates a instace of the vulkan API with validation layers if in debug mode
*/
void Kos::KDevice::CreateInstance()
{
	VkApplicationInfo AppInfo{};


	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pNext = VK_NULL_HANDLE;
	AppInfo.pApplicationName = "KOS Engine"; //Name of the applcation
	AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); //Version of the applcation
	AppInfo.pEngineName = "KOS"; //Name of engine used to make the applcation
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); //Version of the engine used to make the applcation
	AppInfo.apiVersion = VK_API_VERSION_1_4; //Version of the vulkan instance

	//The parameters for the newly created vulkan instance
	VkInstanceCreateInfo InstanceInfo{};

	std::vector<const char*> layers =
	{
		"VK_LAYER_KHRONOS_validation"
		"VK_EXT_debug_utils"
	};

	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pApplicationInfo = &AppInfo;
	InstanceInfo.pNext = VK_NULL_HANDLE;
	InstanceInfo.enabledExtensionCount = 0;
	InstanceInfo.ppEnabledExtensionNames = nullptr;

#ifndef _DEBUG
	if (CheckVaildationLayersSupport(layers))
	{
		InstanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		InstanceInfo.ppEnabledLayerNames = layers.data();
	}
#endif // !NDEBUG

	if (vkCreateInstance(&InstanceInfo, nullptr, &m_instance) != VK_SUCCESS)
	{
		//Replace with a KLog
	}
}

/*
* Creates a surface for a win32 OS
* Currently only supports windows
* 
* uses a unqiue pointer to get access to the window handle and instance
*/
void Kos::KDevice::CreateSurface()
{

	m_window = std::make_unique<KWindow>();

	VkWin32SurfaceCreateInfoKHR surface_info{};

	surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_info.hwnd = m_window->GetWindowHandle(); //Handle to win32 window
	surface_info.hinstance = m_window->GetWindowInstance();//Instance of the win32 window

	if (vkCreateWin32SurfaceKHR(m_instance, &surface_info, nullptr, &m_surface) != VK_SUCCESS)
	{
		//Replace with a KLog
	}
}

/*
* Finds the end user GPU to than ensure it supports all the capabilites of the engine
*/
void Kos::KDevice::FindUsersGPU()
{

}
