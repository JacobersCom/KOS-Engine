#include "KDevice.hpp"
#include "KSwapchain.hpp"
#include "KWindow.hpp"

namespace Kos
{
	namespace
	{
		/*These variables are not needed by any another class or file*/
		VkQueue k_graphics_queue;
		VkQueue k_present_queue;

		std::vector<const char*> layers =
		{
			"VK_LAYER_KHRONOS_validation"
			"VK_EXT_debug_utils"
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
		std::vector<const char*> CheckDeviceExtensionSupport(VkPhysicalDevice _VkPhysicalDevice)
		{
			uint32_t extensionCount;

			vkEnumerateDeviceExtensionProperties(_VkPhysicalDevice, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(_VkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

			std::vector<const char* > wantedExtensions
			{
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			for (const auto& ae : availableExtensions)
			{
				bool extensionsFound = false;
			
				for (const auto& we : wantedExtensions)
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

			return wantedExtensions;
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
					indices.GraphicsFamily = i;
				}

				vkGetPhysicalDeviceSurfaceSupportKHR(phy_device, i, surface, &presentSupport);

				if (presentSupport)
				{
					indices.PresentFamily = i;
				}

				if (indices.isComplete()) break;
				i++;
			}

			return indices;
		}

		Kos::QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice phy_device, VkSurfaceKHR surface)
		{
			QueueFamilyIndices indices = FindQueueFamilies(phy_device, surface);
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
		

		return Indices.isComplete();
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
		VkBool32 presentSupport = false;
		for (const auto& queueFamily : queueFamilys)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.GraphicsFamily = i;
			}

			vkGetPhysicalDeviceSurfaceSupportKHR(phy_device, i,surface, &presentSupport);

			if (presentSupport)
			{
				indices.PresentFamily = i;
			}

			if (indices.isComplete()) break;
			i++;
		}

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

		VkWin32SurfaceCreateInfoKHR surface_info{};

		surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surface_info.hwnd = k_window.GetWindowHandle(); //Handle to win32 window
		surface_info.hinstance = k_window.GetWindowInstance();//Instance of the win32 window

		if (vkCreateWin32SurfaceKHR(m_instance, &surface_info, nullptr, &m_surface) != VK_SUCCESS)
		{
			//Replace with a KLog
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
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			throw std::runtime_error("No device with vulkan support found!");
		}

		//Get device information
		std::vector<VkPhysicalDevice> physical_devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, physical_devices.data());

		//Find a suitable device with vulkan support
		for (const auto physical_device : physical_devices)
		{
			if (RateDeviceSuitable(physical_device, m_surface))
			{
				m_physical_device = physical_device;
				break;
			}
		}

		if (m_physical_device == VK_NULL_HANDLE)
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

		QueueFamilyIndices indices = GetQueueFamilyIndices(m_physical_device, m_surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

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
			VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
		};
		DeviceInfo.enabledExtensionCount = static_cast<uint32_t>(device_ext.size());
		DeviceInfo.ppEnabledExtensionNames = device_ext.data();
#ifdef _DEBUG
		DeviceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		DeviceInfo.ppEnabledLayerNames = layers.data();
#else
	DeviceInfo.enabledLayerCount = 0;
#endif // _DEBUG

		if (vkCreateDevice(m_physical_device, &DeviceInfo, nullptr, &m_device) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create logical device!");
		}

		vkGetDeviceQueue(m_device, indices.GraphicsFamily.value(), 0, &k_graphics_queue);
		vkGetDeviceQueue(m_device, indices.PresentFamily.value(), 0, &k_present_queue);
	}

	/*
	RenderPassInfo is need to discarible how many depth, color buffers there will be and how
	many samples to use for each of them and how there contents should be handle during the
	rendering process

	A RenderPass allows for operations to be performed on a image to improve the quaitily of the final image
*/
	VkRenderPassCreateInfo KDevice::CreateRenderPassInfo(VkFormat format)
	{
		//The description of an color attachment
		VkAttachmentDescription color_desc{};
		color_desc.format = format; //Format of the image view that will be used
		color_desc.samples = VK_SAMPLE_COUNT_1_BIT; //Each pixel's depth, coverage, and stencil will be tested once

		//These value will describle how the contents color and depth are treated before and after pass
		color_desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //Values will become const at the start
		color_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Write contents to memory to read later

		//These value will describle how the contents of the stencil components will be treated before and after pass
		color_desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; //Do not save the content within the render area
		color_desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //Content is not needed after rendering

		//The layout of the image from beginning to end of pass
		color_desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //Image layout unknown and not saved.
		color_desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //Images in the swapchain

		//Simply a reference to the ColorAttachment above
		VkAttachmentReference color_ref{};
		color_ref.attachment = 0; //The index is found in the shader code COLOR0
		color_ref.layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL; //Gives better performance

		//Subpasses allow for different effects to be grouped togther in a renderpass to reorder the operations to save on memory and give better performance
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //Tells Vulkan this is a graphics subpass
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_ref;

		//SubpassDependencys manage the transition of the image layouts
		VkSubpassDependency subpass_deps{};

		subpass_deps.srcSubpass = VK_SUBPASS_EXTERNAL; //Ensures the subpass begins before vkCmdBeginRenderpass
		subpass_deps.dstSubpass = 0;

		subpass_deps.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;//The subpass will wait to till the image is done reading from the swapchain
		subpass_deps.srcAccessMask = 0;

		//Where the dst commands will have access to memory
		subpass_deps.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpass_deps.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		//Linking it all togther
		VkRenderPassCreateInfo renderpass_info{};
		renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderpass_info.attachmentCount = 1;
		renderpass_info.pAttachments = &color_desc;
		renderpass_info.subpassCount = 1;
		renderpass_info.pSubpasses = &subpass;
		renderpass_info.dependencyCount = 1;
		renderpass_info.pDependencies = &subpass_deps;

		VkResult result = vkCreateRenderPass(m_device, &renderpass_info, nullptr, &m_renderpass);
		if (result != VK_SUCCESS)
		{
			printf("Failed to create RenderPass");
		}

		return renderpass_info;
	}

	/*
	Creates a FrameBuffer for all the images that correspond to
	the retrieved image at drawing time.

	Framebuffers are references to VkImageView objects that
	represent attachments
	*/
	void KDevice::CreateFrameBuffers(std::vector<VkImageView> image_views, VkExtent2D extent)
	{

		m_frame_buffers.resize(image_views.size());

		//Iterate through the Imageviews to create a framebuffer form them
		for (int i = 0; i < image_views.size(); i++)
		{
			//Lists of attachements for each _VkSawpchainImageview
			VkImageView attachments[] = {
				image_views[i]
			};

			VkFramebufferCreateInfo FramebufferInfo{};
			FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferInfo.renderPass = m_renderpass;
			FramebufferInfo.attachmentCount = 1;
			FramebufferInfo.pAttachments = attachments;
			FramebufferInfo.height = extent.height;
			FramebufferInfo.width = extent.width;
			FramebufferInfo.layers = 1; //the total sides of an image

			if (vkCreateFramebuffer(m_device, &FramebufferInfo, nullptr, &m_frame_buffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create Framebuffer for Swapchain Image view");
			}
		}

	}


	/*
	Will need a rewrite once a memory allocater is being used
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

		if (vkCreateBuffer(m_device, &buffer_info, nullptr, &buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create vertexbuffer");
		}

		VkMemoryRequirements mem_requirememnts{};
		vkGetBufferMemoryRequirements(m_device, buffer, &mem_requirememnts);

		VkMemoryAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize = mem_requirememnts.size;
		alloc_info.memoryTypeIndex = FindMemoryType(m_physical_device, 
			mem_requirememnts.memoryTypeBits, properties);

		if (vkAllocateMemory(m_device, &alloc_info, nullptr, &buffer_memeory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate vertex buffer memory");
		}

		vkBindBufferMemory(m_device, buffer, buffer_memeory, 0);
	}

	void KDevice::CreateCommandPool(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool command_pool)
	{
		QueueFamilyIndices Indices = GetQueueFamilyIndices(physical_device);

		VkCommandPoolCreateInfo CommandPoolInfo{};
		CommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		//allows the command buffer to record all the commands but if a reset happens it will have to rerecord all the commands
		CommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CommandPoolInfo.queueFamilyIndex = Indices.GraphicsFamily.value();

		if (vkCreateCommandPool(device, &CommandPoolInfo, nullptr, &command_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool");
		}

	}

	void KDevice::CreatePrimaryCommandBuffer()
	{
		VkCommandBufferAllocateInfo AllocateInfo{};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = m_command_pool;
		AllocateInfo.commandBufferCount = 1;

		//Can be submitted to queue but not to another command buffer
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(m_device, &AllocateInfo, &m_command_buffer))
		{
			throw std::runtime_error("Failed to create primary command buffer");
		}
	}

	void KDevice::SyncObjects()
	{
		VkSemaphoreCreateInfo SemaphoreInfo{};
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo FenceInfo{};
		FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; //fences start signed

		if (vkCreateSemaphore(m_device, &SemaphoreInfo, nullptr, &m_image_available) != VK_SUCCESS
			|| vkCreateSemaphore(m_device, &SemaphoreInfo, nullptr, &m_render_finished) != VK_SUCCESS
			|| vkCreateFence(m_device, &FenceInfo, nullptr, &m_frames_in_flight) != VK_SUCCESS
			)
		{
			throw std::runtime_error("Failed to create semaphores and fence!");
		}
	}


}