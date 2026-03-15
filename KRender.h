#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <iostream>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <optional>
#include <set>
#include <fstream>

#include "vulkan/vulkan.h"


namespace KE
{
	enum class KReturn : int;
}
namespace KE::SYSTEM
{
	class KWindow;
}

#ifdef NDEBUG
static const bool enableValidationLayers = false;
#else 
static const bool enableValidationLayers = true;
#endif

namespace KE::RENDERER
{

	struct QueueFamilyIndices {

		std::optional<uint32_t>GraphicsFamily;
		std::optional<uint32_t>PresentFamily;


		bool isComplete()
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR SurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> ImageFormats;
		std::vector<VkPresentModeKHR> PresentMode;
	};

	class KRender
	{
	public:

		KRender(KE::SYSTEM::KWindow& _win) : _win(_win){}

		void run();

		const VkInstance GetVkInstance() { return _VkInstance; }
		
	private:

		KE::KReturn InitVulkan();
		
		void UpdateLoop();
		void CleanUp();
		
		

		KE::KReturn CreateVkInstance(VkInstance& _Instance);
		KE::KReturn PickPhysicalDevice(VkPhysicalDevice& _VkPhysicalDevice, VkInstance _VkInstance);
		KE::KReturn CreateWin32Surface(KE::SYSTEM::KWindow& _win, VkInstance _VkInstance, VkSurfaceKHR& _VkSurfaceKHR);
		KE::KReturn CreateLogicalDevice(VkPhysicalDevice _VkPhysicalDevice, VkDevice& _VkDevice);
		KE::KReturn CreateSwapChain(VkPhysicalDevice _VkPhysicalDevice, VkDevice _VkDevice, VkSwapchainKHR& _VkSwapChain);
		KE::KReturn CreateImageViews(VkDevice _VkDevice);
		KE::KReturn CreatePipeLine();
		KE::RENDERER::QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice);
		KE::RENDERER::QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice);
		
		//PipeLine Helpers
		VkPipelineDynamicStateCreateInfo CreateDynaminceStateInfo(int DynamicStateCount, VkDynamicState* DynamicStateData);
		VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo();
		VkPipelineInputAssemblyStateCreateInfo CreateAssemblyInputStateInfo();
		VkPipelineViewportStateCreateInfo CreateViewPort();
		VkPipelineRasterizationStateCreateInfo CreateRasterizationState();
		VkPipelineColorBlendAttachmentState CreateColorBlendAttachmentState();

		//All SwapChain helpers
		KE::RENDERER::SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice _VkPhysicalDevice);
		VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR> formats);
		VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities);


		//All Shader helpers
		//Takes in the a spv compiled shader
		std::vector<char> LoadShader(const std::string& _FileName);
		//Takes in the return from LoadShaders
		VkShaderModule CreateShaderModule(const std::vector<char>& code);

		//Checks for Device suitability and extension support
		bool IsDeviceSuitable(VkPhysicalDevice _VkPhyscialDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice _VkPhysicalDevice);
		
		//Helpers for required information
		std::vector<const char*> GetRequiredInstanceExtensions();
		std::vector<const char*> GetRequiredInstaceLayers();
		std::vector<const char*> GetRequiredDeviceExtensions();

	private:

		KE::SYSTEM::KWindow& _win;
		VkInstance _VkInstance;
		VkDevice _VkDevice;
		VkQueue _VkGraphicsQueue;
		VkQueue _VkPresentationQueue;
		VkSurfaceKHR _VkSurface;
		VkPhysicalDevice _VkPhysicalDevice;
		VkSwapchainKHR _VkSwapChain;
		VkFormat _VkSwapChainFormat;
		VkExtent2D _VkSwapChainExtent;
		VkPipelineLayout _VkPipelineLayout;



		std::vector<const char*> validationLayers;
		std::vector<const char*> InstanceExtensions;
		std::vector<const char*> deviceExtensions;
		std::vector<VkImage> SwapChainImages;
		std::vector<VkImageView> ImageViews;
		std::vector<VkDynamicState> DynamicStates;
	};
}