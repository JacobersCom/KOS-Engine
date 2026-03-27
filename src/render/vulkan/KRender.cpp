#include <render/vulkan/KRender.h>
#include <platform/win32/KWindow.h>

namespace KE
{
	namespace RENDERER
	{

		void KRender::run()
		{
			InitVulkan();
			UpdateLoop();
			CleanUp();
		}

		void KRender::InitVulkan() 
		{
			KE::RENDERER::KRender::CreateVkInstance();
			KE::RENDERER::KRender::CreateWin32Surface();
			KE::RENDERER::KRender::PickPhysicalDevice();
			KE::RENDERER::KRender::CreateLogicalDevice();
			KE::RENDERER::KRender::CreateSwapChain();
			KE::RENDERER::KRender::CreatePipeline();
		}

		void KRender::UpdateLoop()
		{

		}

		/*
		*/
		void KRender::CleanUp()
		{

			vkDestroySurfaceKHR(_VkInstance, _VkSurface, nullptr);
			vkDestroySwapchainKHR(_VkDevice, _VkSwapchain, nullptr);
			for (auto& ImageView : ImageViews)
			{
				vkDestroyImageView(_VkDevice, ImageView, nullptr);
			}
			vkDestroyRenderPass(_VkDevice, _VkRenderPass, nullptr);
			vkDestroyPipelineLayout(_VkDevice, _VkPipelineLayout, nullptr);
			vkDestroyDevice(_VkDevice, nullptr);
		}

		/*
		*/
		void KRender::CreateVkInstance()
		{
			//Applcation information
			VkApplicationInfo AppInfo{};

			AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			AppInfo.pNext = VK_NULL_HANDLE;
			AppInfo.pApplicationName = "KOS Engine";
			AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			AppInfo.pEngineName = "KOS";
			AppInfo.apiVersion = VK_API_VERSION_1_4;

			VkInstanceCreateInfo InstanceInfo{};

			InstanceExtensions = GetRequiredInstanceExtensions();

			InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceInfo.pApplicationInfo = &AppInfo;
			InstanceInfo.pNext = VK_NULL_HANDLE;
			InstanceInfo.enabledExtensionCount = static_cast<uint32_t>(InstanceExtensions.size());
			InstanceInfo.ppEnabledExtensionNames = InstanceExtensions.data();

			if (enableValidationLayers && !CheckVaildationLayerSupport())
			{
				throw std::runtime_error("Validation layers requested, but not available");
			}
			else
			{
				InstanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				InstanceInfo.ppEnabledLayerNames = validationLayers.data();
			}

			VkResult result = vkCreateInstance(&InstanceInfo, nullptr, &_VkInstance);
		
			if (vkCreateInstance(&InstanceInfo, nullptr, &_VkInstance) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create VkInstance");
			}

		}

		/*
		*/
		void KRender::CreateWin32Surface()
		{

			VkWin32SurfaceCreateInfoKHR _WinSurfaceInfo{};

			_WinSurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			_WinSurfaceInfo.hwnd = _win->GetWindowHandle();
			_WinSurfaceInfo.hinstance = _win->GetWindowInstance();

			if (vkCreateWin32SurfaceKHR(_VkInstance, &_WinSurfaceInfo, nullptr, &_VkSurface) != VK_SUCCESS)
			{
				throw std::runtime_error("Win32 Surface creation failed");
			}
		}

		/*
		*/
		void KRender::PickPhysicalDevice()
		{
			//Get device count
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(_VkInstance, &deviceCount, nullptr);

			if (deviceCount == 0)
			{
				throw std::runtime_error("No device with vulkan support found!");
			}

			//Get device information
			std::vector<VkPhysicalDevice> PhysicalDevices(deviceCount);
			vkEnumeratePhysicalDevices(_VkInstance, &deviceCount, PhysicalDevices.data());

			//Find a suitable device with vulkan support
			for (const auto PhysicalDevice : PhysicalDevices)
			{
				if (IsDeviceSuitable(PhysicalDevice))
				{
					_VkPhysicalDevice = PhysicalDevice;
					break;
				}
			}

			if (_VkPhysicalDevice == VK_NULL_HANDLE)
			{
				throw std::runtime_error("Failed to find Suitable GPU");
			}

		}

		/*
		*/
		KE::RENDERER::KRender::QueueFamilyIndices KRender::FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice)
		{
			QueueFamilyIndices indices;

			//Get the properties count
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(_VkPhysicalDevice, &queueFamilyCount, nullptr);

			//Get the properties data
			std::vector<VkQueueFamilyProperties> queueFamilys(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(_VkPhysicalDevice, &queueFamilyCount, queueFamilys.data());

			//Find the graphics bit queue family
			int i = 0; 
			VkBool32 presentSupport = false;
			for (const auto& queueFamily : queueFamilys)
			{
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					indices.GraphicsFamily = i;
				}

				vkGetPhysicalDeviceSurfaceSupportKHR(_VkPhysicalDevice, i, _VkSurface, &presentSupport);

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
		*/
		KE::RENDERER::KRender::QueueFamilyIndices KRender::GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice)
		{
			QueueFamilyIndices indices = FindQueueFamilies(_VkPhysicalDevice);
			return indices;
		}

		/*
		*/
		VkPipelineDynamicStateCreateInfo KRender::CreateDynaminceStateInfo(int DynamicStateCount, VkDynamicState* DynamicStateData)
		{
			VkPipelineDynamicStateCreateInfo DynamicStateInfo{};
			DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			DynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(DynamicStateCount);
			DynamicStateInfo.pDynamicStates = DynamicStateData;

			return DynamicStateInfo;
		}

		/*
		*/
		VkPipelineVertexInputStateCreateInfo KRender::CreateVertexInputStateInfo()
		{
			VkPipelineVertexInputStateCreateInfo VertexStateInfo{};
			VertexStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			VertexStateInfo.pVertexAttributeDescriptions = nullptr;
			VertexStateInfo.pVertexBindingDescriptions = nullptr;
			VertexStateInfo.vertexAttributeDescriptionCount = 0;
			VertexStateInfo.vertexBindingDescriptionCount = 0;
			return VertexStateInfo;
		}

		/*
		*/
		VkPipelineInputAssemblyStateCreateInfo KRender::CreateAssemblyInputStateInfo()
		{
			VkPipelineInputAssemblyStateCreateInfo AssemblyStateInfo{};
			AssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			AssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			AssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

			return AssemblyStateInfo;
		}

		/*
		*/
		VkPipelineViewportStateCreateInfo KRender::CreateViewPort()
		{
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(_VkSwapchainExtent.width);
			viewport.height = static_cast<float>(_VkSwapchainExtent.height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = _VkSwapchainExtent;

			VkPipelineViewportStateCreateInfo ViewPortInfo{};
			ViewPortInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			ViewPortInfo.viewportCount = 1;
			ViewPortInfo.pViewports = &viewport;
			ViewPortInfo.scissorCount = 1;
			ViewPortInfo.pScissors = &scissor;

			return ViewPortInfo;
		}

		/*
		*/
		VkPipelineRasterizationStateCreateInfo KRender::CreateRasterizationState()
		{
			VkPipelineRasterizationStateCreateInfo RasterStateInfo{};
			RasterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			RasterStateInfo.depthClampEnable = VK_FALSE; // Using this requires enabling VkPipelineRasterizationDepthClipStateCreateInfoEXT within the pipeline
			RasterStateInfo.rasterizerDiscardEnable = VK_FALSE; // if trust it stops everything from reaching the frame buffer
			RasterStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
			RasterStateInfo.lineWidth = 1.f; //Anything thicker than 1.f requries a gpu feature
			//Determines the type of face culling to use
			RasterStateInfo.cullMode = VK_CULL_MODE_BACK_BIT; 
			RasterStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

			RasterStateInfo.depthBiasEnable = VK_FALSE;
			RasterStateInfo.depthBiasConstantFactor = 0.0f; 
			RasterStateInfo.depthBiasClamp = 0.0f; 
			RasterStateInfo.depthBiasSlopeFactor = 0.0f; 

			return RasterStateInfo;
		}

		/*
		*/
		VkPipelineColorBlendAttachmentState KRender::CreateColorBlendAttachmentState()
		{
			//Alpha blending
			VkPipelineColorBlendAttachmentState ColorBlendState{};
			ColorBlendState.blendEnable = VK_TRUE;
			ColorBlendState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			ColorBlendState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			ColorBlendState.colorBlendOp = VK_BLEND_OP_ADD;
			ColorBlendState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			ColorBlendState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			ColorBlendState.alphaBlendOp = VK_BLEND_OP_ADD;

			return  ColorBlendState;
		}

		/*
		 RenderPassInfo is need to discarible how many depth, color buffers there will be and how
		 many samples to use for each of them and how there contents should be handle during the 
		 rendering process

		 A RenderPass allows for operations to be performed on a image to improve the quaitily of the final image
		*/
		VkRenderPassCreateInfo KRender::CreateRenderPassInfo()
		{
			//The description of an color attachment
			VkAttachmentDescription ColorAttachment{};
			ColorAttachment.format = _VkSwapchainFormat; //Format of the image view that will be used
			ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; //Each pixel's depth, coverage, and stencil will be tested once

			//These value will describle how the contents color and depth are treated before and after pass
			ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //Values will become const at the start
			ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Write contents to memory to read later

			//These value will describle how the contents of the stencil components will be treated before and after pass
			ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; //Do not save the content within the render area
			ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //Content is not needed after rendering

			//The layout of the image from beginning to end of pass
			ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //Image layout unknown and not saved.
			ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //Images in the swapchain

			//Simply a reference to the ColorAttachment above
			VkAttachmentReference ColorAttachmentRef{};
			ColorAttachmentRef.attachment = 0; //The index is found in the shader code COLOR0
			ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL; //Gives better performance

			//Subpasses allow for different effects to be grouped togther in a renderpass to reorder the operations to save on memory and give better performance
			VkSubpassDescription Subpass{};
			Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //Tells Vulkan this is a graphics subpass
			Subpass.colorAttachmentCount = 0; 
			Subpass.pColorAttachments = &ColorAttachmentRef;

			//Linking it all togther
			VkRenderPassCreateInfo RenderPassInfo{};
			RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			RenderPassInfo.attachmentCount = 1;
			RenderPassInfo.pAttachments = &ColorAttachment;
			RenderPassInfo.subpassCount = 1;
			RenderPassInfo.pSubpasses = &Subpass;

			VkResult result = vkCreateRenderPass(_VkDevice, &RenderPassInfo, nullptr, &_VkRenderPass);
			if (result != VK_SUCCESS)
			{
				printf("Failed to create RenderPass");
			}

			return RenderPassInfo;
		}

		/*
		*/
		KE::RENDERER::KRender::SwapChainSupportDetails KRender::GetSwapChainDetails(VkPhysicalDevice _VkPhysicalDevice)
		{
			SwapChainSupportDetails SwapChainDetails;

			//Surface Capabilities
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_VkPhysicalDevice, _VkSurface, &SwapChainDetails.SurfaceCapabilities);

			//Format count
			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(_VkPhysicalDevice, _VkSurface, &formatCount, nullptr);

			if (formatCount != 0)
			{
				SwapChainDetails.ImageFormats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(_VkPhysicalDevice, _VkSurface, &formatCount, SwapChainDetails.ImageFormats.data());
			}

			//Presentation modes
			uint32_t presentCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(_VkPhysicalDevice, _VkSurface, &presentCount, nullptr);

			if (presentCount != 0)
			{
				SwapChainDetails.PresentMode.resize(presentCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(_VkPhysicalDevice, _VkSurface, &presentCount, SwapChainDetails.PresentMode.data());
			}

			return SwapChainDetails;
		}

		/*
		*/
		VkSurfaceFormatKHR KRender::ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR> formats)
		{
			for (const auto availableFormat : formats)
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB //the format encoding
					&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) // applys support for images in SRGB color space
				{
					return availableFormat;
				}

				return formats[0];
			}
		}

		/*
		*/
		VkPresentModeKHR KRender::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
		{
			for (const auto& availablePresentMode : presentModes)
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
					return availablePresentMode;
			}
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		/*
		*/
		VkExtent2D KRender::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities)
		{
			if (capabilities.currentExtent.width != UINT32_MAX)
			{
				return capabilities.currentExtent;
			}
			else 
			{
				int width, height;
				_win->GetFrameBufferSize(_win->GetWindowHandle(), width, height);

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

		/*
		*/
		void KE::RENDERER::KRender::CreateLogicalDevice()
		{
			//Ranges between 0.0 - 1.0
			float QueuePriority = 1.0f;

			std::vector<const char*> extentions = GetRequiredInstanceExtensions();
			QueueFamilyIndices indices = GetQueueFamilyIndices(_VkPhysicalDevice);

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

			deviceExtensions = GetRequiredDeviceExtensions();

			VkDeviceCreateInfo DeviceInfo{};
			DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			DeviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			DeviceInfo.pQueueCreateInfos = queueCreateInfos.data();
			DeviceInfo.pEnabledFeatures = &DeviceFeaturesInfo;
			DeviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
			DeviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

			if (enableValidationLayers)
			{
				DeviceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				DeviceInfo.ppEnabledLayerNames = validationLayers.data();
			}
			else
			{
				DeviceInfo.enabledLayerCount = 0;
			}

			VkResult result = vkCreateDevice(_VkPhysicalDevice, &DeviceInfo, nullptr, &_VkDevice);

			if (result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create logical device!");
			}

			vkGetDeviceQueue(_VkDevice, indices.GraphicsFamily.value(), 0, &_VkGraphicsQueue);
			vkGetDeviceQueue(_VkDevice, indices.GraphicsFamily.value(), 0, &_VkPresentationQueue);


		}

		/*
		*/
		void KRender::CreateSwapChain()
		{
			SwapChainSupportDetails SwapChainDetails = GetSwapChainDetails(_VkPhysicalDevice);

			VkSurfaceFormatKHR SurfaceFormat = ChooseSwapChainFormat(SwapChainDetails.ImageFormats);
			VkPresentModeKHR PresentMode = ChooseSwapChainPresentMode(SwapChainDetails.PresentMode);
			VkExtent2D Extent = ChooseSwapExtent(SwapChainDetails.SurfaceCapabilities);
		
			uint32_t ImageCount = SwapChainDetails.SurfaceCapabilities.minImageCount + 1;

			if (SwapChainDetails.SurfaceCapabilities.minImageCount > 0 && ImageCount > SwapChainDetails.SurfaceCapabilities.maxImageCount)
			{
				ImageCount = SwapChainDetails.SurfaceCapabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR SwapChainInfo{};
			SwapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			SwapChainInfo.surface = _VkSurface;
			SwapChainInfo.minImageCount = ImageCount;
			SwapChainInfo.imageFormat = SurfaceFormat.format;
			SwapChainInfo.imageColorSpace = SurfaceFormat.colorSpace;
			SwapChainInfo.imageExtent = Extent;
			SwapChainInfo.imageArrayLayers = 1;
			SwapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices Indices = FindQueueFamilies(_VkPhysicalDevice);
			uint32_t queueFamilyIndices[] = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };

			if (Indices.GraphicsFamily != Indices.PresentFamily)
			{
				SwapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Image can be used by muiltple queues
				SwapChainInfo.queueFamilyIndexCount = 2;
				SwapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else
			{
				SwapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Images can only be used by a single queue
				SwapChainInfo.queueFamilyIndexCount = 0; 
				SwapChainInfo.pQueueFamilyIndices = nullptr; 
			}

			SwapChainInfo.preTransform = SwapChainDetails.SurfaceCapabilities.currentTransform;
			SwapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // ALPHA should not be used for blending windows
			SwapChainInfo.presentMode = PresentMode;
			SwapChainInfo.clipped = VK_TRUE; // Dont care about covered pixels
			SwapChainInfo.oldSwapchain = VK_NULL_HANDLE;
		
			VkResult result = vkCreateSwapchainKHR(_VkDevice, &SwapChainInfo, nullptr, &_VkSwapchain);

			if (result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create SwapChain");
			}

			vkGetSwapchainImagesKHR(_VkDevice, _VkSwapchain, &ImageCount, nullptr);
			SwapChainImages.resize(ImageCount);
			vkGetSwapchainImagesKHR(_VkDevice, _VkSwapchain, &ImageCount, SwapChainImages.data());

			_VkSwapchainFormat = SurfaceFormat.format;
			_VkSwapchainExtent = Extent;

		}

		/*
		*/
		void KRender::CreateImageViews()
		{
			ImageViews.resize(SwapChainImages.size());
		
			for (int i = 0; i < SwapChainImages.size(); i++)
			{
				VkImageViewCreateInfo ImageViewInfo{};
				ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				ImageViewInfo.image = SwapChainImages[i];
				ImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				ImageViewInfo.format = _VkSwapchainFormat;
				ImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				ImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				ImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				ImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			
				ImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				ImageViewInfo.subresourceRange.baseMipLevel = 0;
				ImageViewInfo.subresourceRange.levelCount = 1;
				ImageViewInfo.subresourceRange.baseArrayLayer = 0;
				ImageViewInfo.subresourceRange.layerCount = 1;

				VkResult result = vkCreateImageView(_VkDevice, &ImageViewInfo, nullptr, &ImageViews[i]);

				if (result != VK_SUCCESS)
				{
					throw std::runtime_error("Failed to create image views for swapchain");
				}
			}
		}

		/*
		*/
		void KRender::CreatePipeline()
		{
			auto VertShaderCode = LoadShader("VertShader.spv");
			auto PixelShaderCode = LoadShader("frag.spv");
		
			VkShaderModule VertModule = CreateShaderModule(VertShaderCode);
			VkShaderModule PixelModule = CreateShaderModule(PixelShaderCode);


			VkPipelineShaderStageCreateInfo VertexStage{};
			VertexStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			VertexStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
			VertexStage.module = VertModule;
			VertexStage.pName = "main";

			VkPipelineShaderStageCreateInfo PixelStage{};
			VertexStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			VertexStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			VertexStage.module = PixelModule;
			VertexStage.pName = "main";

			VkPipelineShaderStageCreateInfo ShaderStages[] = { VertexStage, PixelStage };

			VkPipelineVertexInputStateCreateInfo VertexInput = CreateVertexInputStateInfo();

			VkPipelineInputAssemblyStateCreateInfo AssemblyInput = CreateAssemblyInputStateInfo();

			DynamicStates = {
				VK_DYNAMIC_STATE_SCISSOR,
				VK_DYNAMIC_STATE_VIEWPORT
			};

			VkPipelineDynamicStateCreateInfo DynamicStateInfo = CreateDynaminceStateInfo(DynamicStates.size(), DynamicStates.data());

			VkPipelineLayoutCreateInfo PipelineInfo{};
			PipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			PipelineInfo.setLayoutCount = 0;
			PipelineInfo.pSetLayouts = nullptr;
			PipelineInfo.pushConstantRangeCount = 0;
			PipelineInfo.pPushConstantRanges = nullptr;

			VkResult result = vkCreatePipelineLayout(_VkDevice, &PipelineInfo, nullptr, &_VkPipelineLayout);

			if (result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create pipeline layout");
			}

			//Clean up
			vkDestroyShaderModule(_VkDevice, VertModule, nullptr);
			vkDestroyShaderModule(_VkDevice, PixelModule, nullptr);


		}

		/*
		*/
		void KRender::CreateFramebuffers()
		{
			//For each swapchain image there needs to be a frame buffer with it. hence the resize.
			Framebuffers.resize(SwapChainImages.size());

			for (int i = 0; i < SwapChainImages.size(); i++)
			{
				VkImage attachments[] =
				{
					SwapChainImages[i]
				};
			}

			VkFramebufferCreateInfo FrameBufferInfo{};
			FrameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			
		}

		/*
		*/
		std::vector<const char*> KRender::GetRequiredInstanceExtensions()
		{
			std::vector<const char*> extensions;

			if (enableValidationLayers)
			{
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

			return extensions;
		}

		/*
		*/
		std::vector<const char*> KRender::GetRequiredInstaceLayers()
		{
			std::vector<const char*> layers;
		
			if (enableValidationLayers)
			{
				layers.push_back("VK_LAYER_KHRONOS_validation");
			}
			return layers;
		}

		/*
		*/
		std::vector<const char*> KRender::GetRequiredDeviceExtensions()
		{
			std::vector<const char*> extensions;
			extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

			return extensions;
		}

		/*
		*/
		bool KRender::IsDeviceSuitable(VkPhysicalDevice _VkPhyscialDevice)
		{
			QueueFamilyIndices Indices = KRender::FindQueueFamilies(_VkPhyscialDevice);

			bool extensionsSupported = CheckDeviceExtensionSupport(_VkPhyscialDevice);

			//Is the SwapChain supported
			bool SwapChainAdequate = false;
			if (extensionsSupported)
			{
				SwapChainSupportDetails SwapChainSupportDetails = GetSwapChainDetails(_VkPhyscialDevice);
				SwapChainAdequate = !SwapChainSupportDetails.ImageFormats.empty() && !SwapChainSupportDetails.PresentMode.empty();
			}

			return Indices.isComplete() && SwapChainAdequate && extensionsSupported;
		}

		/*
		*/
		std::vector<char> KRender::LoadShader(const std::string& _FileName)
		{
			//Starts reading at the end of the file
			std::ifstream file(_FileName, std::ios::ate | std::ios::binary);

			file.open(_FileName);
			
			if (!file.is_open())
			{
				printf("Failed to load shader files");
				throw std::runtime_error("Failed to load shader files");

			}

			//Returns the files size in bytes
			auto fileSize = file.tellg();
			std::vector<char> buffer(fileSize);

			//Reset the postion pointer to the beginnering of the file
			file.seekg(0);
			//Read shader data into vector
			file.read(buffer.data(), fileSize);

			file.close();
			return buffer;
		}

		/*
		*/
		bool KRender::CheckDeviceExtensionSupport(VkPhysicalDevice _VkPhysicalDevice)
		{
			uint32_t extensionCount;

			vkEnumerateDeviceExtensionProperties(_VkPhysicalDevice, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(_VkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

			for (const auto& extension : availableExtensions)
			{
				requiredExtensions.erase(extension.extensionName);
			}
			return requiredExtensions.empty();
		}

		/*
		*/
		bool KRender::CheckVaildationLayerSupport()
		{
			uint32_t LayerCount;
			vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(LayerCount);
			vkEnumerateInstanceLayerProperties(&LayerCount, availableLayers.data());

			for (const auto* LayerName : validationLayers)
			{
				bool LayerFound = false;
			
				for (const auto& LayerProperties : availableLayers)
				{
					if(strcmp(LayerName, LayerProperties.layerName) == 0)
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
		*/
		VkShaderModule KRender::CreateShaderModule(const std::vector<char>& code)
		{
			VkShaderModuleCreateInfo ShaderModuleCreateInfo{};
			ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			ShaderModuleCreateInfo.codeSize = code.size();
			ShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

			VkShaderModule _VkShaderModule;

			VkResult result = vkCreateShaderModule(_VkDevice, &ShaderModuleCreateInfo, nullptr, &_VkShaderModule);
			if (result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create Shader Module");
			}

			return _VkShaderModule;
		}
	}
}
