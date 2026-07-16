#include "KDevice.hpp"
#include "KSwapchain.hpp"
#include "KPipeline.hpp"
#include "KWindow.hpp"

namespace Kos
{
	namespace
	{
		/*These variables are not needed by any another class or file*/
		

		std::vector<const char*> layers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

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

		/*
		* Ensure that the GPU has swapchain support. And returns the required extension names
		*/
		bool CheckDeviceExtensionSupport(VkPhysicalDevice _VkPhysicalDevice)
		{
			uint32_t extensionCount;

			vkEnumerateDeviceExtensionProperties(_VkPhysicalDevice, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(_VkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

			std::vector<const char* > wantedExtensions
			{
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			for (const auto& we : wantedExtensions)
			{
				bool extensionsFound = false;
			
				for (const auto& ae : availableExtensions)
				{
					if (strcmp(ae.extensionName, we) == 0)
					{
						extensionsFound = true;
						break;
					}
				}
				if (!extensionsFound)
				{
					throw std::runtime_error("Failed to find required device extensions");
				}
			}
			return true;
		}
		Kos::QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
		{
			QueueFamilyIndices indices;

			//Get the properties count
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(phy_device, &queueFamilyCount, nullptr);

			//Get the properties data
			std::vector<VkQueueFamilyProperties> queueFamilys(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(phy_device, &queueFamilyCount, queueFamilys.data());

			//Find the graphics bit queue family
			int i = 0;
			VkBool32 presentSupport = false;
			for (const auto& queueFamily : queueFamilys)
			{
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					indices.m_graphics_family = i;
				}

				vkGetPhysicalDeviceSurfaceSupportKHR(phy_device, i, surface, &presentSupport);

				if (presentSupport)
				{
					indices.m_present_family = i;
				}

				if (indices.isComplete()) break;
				i++;
			}

			return indices;
		}


		uint32_t FindMemoryType(VkPhysicalDevice physical_device,uint32_t type_filter, VkMemoryPropertyFlags properties)
		{
			VkPhysicalDeviceMemoryProperties mem_properties{};
			vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

			for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
			{
				if (type_filter & (1 << i) && (mem_properties.memoryTypes[i].propertyFlags & properties)
					== properties)
				{
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}

	}

	/*
	Checks the end users GPU for
		- swapchain support
		- Graphics Queue
		- Present Queue

		TODO: Instead of picking the first GPU implment a rating system for them
		TODO: Find a different way of checking for device extensions
	*/
	bool KDevice::RateDeviceSuitable(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
	{
		QueueFamilyIndices Indices = FindQueueFamilies(phy_device, surface);

		return Indices.isComplete() && CheckDeviceExtensionSupport(phy_device);
	}

	/*
	* Finds all the needed queue familys for rendering and presenting the rendered image
	*/
	QueueFamilyIndices KDevice::FindQueueFamilies(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
	{
		QueueFamilyIndices indices;

		//Get the properties count
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(phy_device, &queueFamilyCount, nullptr);

		//Get the properties data
		std::vector<VkQueueFamilyProperties> queueFamilys(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(phy_device, &queueFamilyCount, queueFamilys.data());

		//Find the graphics bit queue family
		int i = 0;
		for (const auto& queueFamily : queueFamilys)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.m_graphics_family = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(phy_device, i,surface, &presentSupport);

			if (presentSupport)
			{
				indices.m_present_family = i;
			}

			if (indices.isComplete())
			{
				break;
			}
			i++;
		}

		return indices;
	}
	
	void Kos::KDevice::startup()
	{
		
		CreateInstance();
		CreateSurface();
		FindUsersGPU();
		CreateLogicDevice();
	}

	QueueFamilyIndices KDevice::GetQueueFamilyIndices()
	{
		QueueFamilyIndices indices = FindQueueFamilies(physical_device, surface);
		return indices;
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

		std::vector<const char*> instance_extension
		{
			VK_KHR_SURFACE_EXTENSION_NAME,      
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME		
		};

		InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		InstanceInfo.pApplicationInfo = &AppInfo;
		InstanceInfo.pNext = VK_NULL_HANDLE;
		InstanceInfo.enabledExtensionCount = instance_extension.size();
		InstanceInfo.ppEnabledExtensionNames = instance_extension.data();

	#ifndef NDEBUG
		if (CheckVaildationLayersSupport(layers))
		{
			InstanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
			InstanceInfo.ppEnabledLayerNames = layers.data();
		}
	#endif // !NDEBUG

		if (vkCreateInstance(&InstanceInfo, nullptr, &instance) != VK_SUCCESS)
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

		VkWin32SurfaceCreateInfoKHR surface_info{};

		surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surface_info.hwnd = m_window->GetWindowHandle(); //Handle to win32 window
		surface_info.hinstance = m_window->GetWindowInstance();//Instance of the win32 window

		VkResult result = vkCreateWin32SurfaceKHR(instance, &surface_info, nullptr, &surface);

		if ( result != VK_SUCCESS)
		{
			//Replace with a KLog
			throw std::runtime_error("Failed to create win32 surface");
		}
	}

	/*
	* Finds the end user GPU to than ensure it supports all the capabilites of the engine
	* 
	* TODO: change error handling to using KLog
	*/
	void KDevice::FindUsersGPU()
	{
		//Get device count
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			throw std::runtime_error("No device with vulkan support found!");
		}

		//Get device information
		std::vector<VkPhysicalDevice> physical_devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, physical_devices.data());

		//Find a suitable device with vulkan support
		for (const auto m_physical_device : physical_devices)
		{
			if (RateDeviceSuitable(m_physical_device, surface))
			{
				physical_device = m_physical_device;
				break;
			}
		}

		if (physical_device == VK_NULL_HANDLE)
		{
			throw std::runtime_error("Failed to find Suitable GPU");
		}
	}

	void Kos::KDevice::CreateLogicDevice()
	{
		//Ranges between 0.0 - 1.0
		float QueuePriority = 1.0f;

		std::vector<const char*> instance_ext =
		{
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME
		};
#ifdef _DEBUG
		instance_ext.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // _DEBUG

		QueueFamilyIndices indices = GetQueueFamilyIndices();

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.m_graphics_family.value(), indices.m_present_family.value() };

		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo DeviceQueueInfo{};
			DeviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			DeviceQueueInfo.pNext = VK_NULL_HANDLE;
			DeviceQueueInfo.queueCount = 1;
			DeviceQueueInfo.queueFamilyIndex = queueFamily;
			DeviceQueueInfo.pQueuePriorities = &QueuePriority;
			queueCreateInfos.push_back(DeviceQueueInfo);
		}


		VkPhysicalDeviceFeatures DeviceFeaturesInfo{};

		//Enabled to allow VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL
		VkPhysicalDeviceSynchronization2FeaturesKHR PhysicalDeviceSync{};
		PhysicalDeviceSync.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR;
		PhysicalDeviceSync.synchronization2 = VK_TRUE;

		VkDeviceCreateInfo DeviceInfo{};
		DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceInfo.pNext = &PhysicalDeviceSync;

		DeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		DeviceInfo.pQueueCreateInfos = queueCreateInfos.data();
		DeviceInfo.pEnabledFeatures = &DeviceFeaturesInfo;

		std::vector<const char*> device_ext =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};
		DeviceInfo.enabledExtensionCount = static_cast<uint32_t>(device_ext.size());
		DeviceInfo.ppEnabledExtensionNames = device_ext.data();
#ifdef _DEBUG
		DeviceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		DeviceInfo.ppEnabledLayerNames = layers.data();
#else
	DeviceInfo.enabledLayerCount = 0;
#endif // _DEBUG

		if (vkCreateDevice(physical_device, &DeviceInfo, nullptr, &device) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create logical device!");
		}

		vkGetDeviceQueue(device, indices.m_graphics_family.value(), 0, &graphics_queue);
		vkGetDeviceQueue(device, indices.m_present_family.value(), 0, &present_queue);
	}

	

	/*
	TODO Will need a rewrite once a memory allocater is being used
	*/
	void Kos::KDevice::CreateBuffer(VkDeviceSize size,
		VkBufferUsageFlags useage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& buffer_memeory)
	{
		VkBufferCreateInfo buffer_info{};
		buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_info.flags = useage;
		buffer_info.size = size;
		buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //access to any resource will be exclusvie to one queue family at a time

		if (vkCreateBuffer(device, &buffer_info, nullptr, &buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create vertexbuffer");
		}

		VkMemoryRequirements mem_requirememnts{};
		vkGetBufferMemoryRequirements(device, buffer, &mem_requirememnts);

		VkMemoryAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize = mem_requirememnts.size;
		alloc_info.memoryTypeIndex = FindMemoryType(physical_device, 
			mem_requirememnts.memoryTypeBits, properties);

		if (vkAllocateMemory(device, &alloc_info, nullptr, &buffer_memeory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate vertex buffer memory");
		}

		vkBindBufferMemory(device, buffer, buffer_memeory, 0);
	}
}