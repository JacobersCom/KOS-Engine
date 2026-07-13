#include "KDevice.hpp"
#include "KSwapchain.hpp"
#include "KPipeline.hpp"
#include "KWindow.hpp"

namespace Kos
{
	namespace
	{
		/*These variables are not needed by any another class or file*/
		VkQueue m_graphics_queue;
		VkQueue m_present_queue;

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
				indices.m_graphics_family = i;
			}

			vkGetPhysicalDeviceSurfaceSupportKHR(phy_device, i,surface, &presentSupport);

			if (presentSupport)
			{
				indices.m_present_family = i;
			}

			if (indices.isComplete()) break;
			i++;
		}

		return indices;
	}
	
	bool Kos::KDevice::startup()
	{
		
		CreateInstance();
		CreateSurface();
		FindUsersGPU();
		CreateLogicDevice();
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
		surface_info.hwnd = m_window->GetWindowHandle(); //Handle to win32 window
		surface_info.hinstance = m_window->GetWindowInstance();//Instance of the win32 window

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

		vkGetDeviceQueue(m_device, indices.m_graphics_family.value(), 0, &m_graphics_queue);
		vkGetDeviceQueue(m_device, indices.m_present_family.value(), 0, &m_present_queue);
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

	/*
	Creates a command pool on the CPU to record commands to be passed into the GPU queue
	*/
	void KDevice::CreateCommandPool(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool command_pool)
	{
		QueueFamilyIndices Indices = GetQueueFamilyIndices(physical_device, m_surface);

		VkCommandPoolCreateInfo CommandPoolInfo{};
		CommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		//allows the command buffer to record all the commands but if a reset happens it will have to rerecord all the commands
		CommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CommandPoolInfo.queueFamilyIndex = Indices.m_graphics_family.value();

		if (vkCreateCommandPool(device, &CommandPoolInfo, nullptr, &command_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool");
		}

	}

	/*
	Creates a buffer that sends commands to the GPU such as draw , state change, memory  transfers
	*/
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

	void Kos::KDevice::RecordCommandBuffers(VkCommandBuffer, uint32_t image_index)
	{
		VkCommandBufferBeginInfo BeginInfo{};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(m_command_buffer, &BeginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to write to commandbuffer");
		}

		VkRenderPassBeginInfo RenderPassBeginInfo{};
		RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassBeginInfo.renderPass = m_renderpass;
		RenderPassBeginInfo.framebuffer = m_frame_buffers[image_index]; //The framebuffer and the attachements 
		//Affected area of the render pass
		RenderPassBeginInfo.renderArea.offset = { 0,0 };
		RenderPassBeginInfo.renderArea.extent = m_swapchain->m_extent;

		VkClearValue ClearValues = { {{0.0f, 0.0f, 0.0f, 1.0f}} }; //Clear color used for VK_ATTACHMENT_LOAD_OP_CLEAR
		RenderPassBeginInfo.clearValueCount = 1;
		RenderPassBeginInfo.pClearValues = &ClearValues;

		//The commands being recorded will be stored here and VK_SUBPASS_CONTENTS_INLINE means the commands
		//will be embedded in the primary command buffer
		vkCmdBeginRenderPass(m_command_buffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipline());

		//Because the viewport and scissor is dynmic so they must be set in the command buffer before drawing

		vkCmdSetViewport(m_command_buffer, 0, 1, &m_pipeline->GetViewport());
		vkCmdSetScissor(m_command_buffer, 0, 1, &m_pipeline->GetScissor());

		//Now we can draw
		vkCmdDraw(m_command_buffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(m_command_buffer);

		if (vkEndCommandBuffer(m_command_buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record to command buffer");
		}
	}

	void KDevice::DrawFrame()
	{
		//Waits for all fences to be signed before returning and disables a time out.
		//All fences start off signed so this is oki
		vkWaitForFences(m_device, 1, &m_frames_in_flight, VK_TRUE, UINT64_MAX);
		vkResetFences(m_device, 1, &m_frames_in_flight);

		uint32_t ImageIndex;

		//imageavailablesemaphore signeds when the presentation engine is finish
		vkAcquireNextImageKHR(m_device, m_swapchain->GetSwapchain(), UINT64_MAX,
			m_image_available, VK_NULL_HANDLE, &ImageIndex);


		//Ensures the buffer is able to record commands
		vkResetCommandBuffer(m_command_buffer, 0);

		//Record info to submit it
		RecordCommandBuffers(m_command_buffer, ImageIndex);

		VkSubmitInfo SubmittedInfo{};
		SubmittedInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		//The Semaphore upon which to wait on before exe the commandbuffer
		VkSemaphore WaitSemaphore[] = { m_image_available };
		SubmittedInfo.waitSemaphoreCount = 1;
		SubmittedInfo.pWaitSemaphores = WaitSemaphore;

		//Where each semaphore will wait for occur. In this case is when the pipeline writes color
		VkPipelineStageFlags Stage[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		SubmittedInfo.pWaitDstStageMask = Stage;

		SubmittedInfo.commandBufferCount = 1;
		SubmittedInfo.pCommandBuffers = &m_command_buffer;

		//Which semaphore to sign once the commandbuffer is finished
		VkSemaphore SingleSemaphore[] = { m_render_finished };
		SubmittedInfo.signalSemaphoreCount = 1;
		SubmittedInfo.pSignalSemaphores = SingleSemaphore;

		//All commands will be submitted to the queue. The fence Will singal when it is finished
		if (vkQueueSubmit(m_graphics_queue, 1, &SubmittedInfo, m_frames_in_flight) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit commands to queue");
		}


		VkPresentInfoKHR PresentInfo{};
		PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		//Program waits on these before presenting
		PresentInfo.waitSemaphoreCount = 1;
		PresentInfo.pWaitSemaphores = SingleSemaphore;

		PresentInfo.swapchainCount = 1;
		PresentInfo.pSwapchains = m_swapchain->GetSwapchain();
		PresentInfo.pImageIndices = &ImageIndex;

		vkQueuePresentKHR(m_present_queue, &PresentInfo);

		vkDeviceWaitIdle(m_device);

	}


}