#pragma once

#pragma region Defines
#define VK_USE_PLATFORM_WIN32_KHR

#ifdef NDEBUG
 const bool enableValidationLayers = false;
#else 
 const bool enableValidationLayers = true;
#endif
#pragma endregion

#pragma region Includes
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <optional>
#include <set>
#include <fstream>
#include <filesystem>

#include <vulkan/vulkan.h>
#pragma endregion

#pragma region Forward Declares
namespace KE::SYSTEM
{
	class KWindow;
}
#pragma endregion


namespace KE 
{
	namespace RENDERER
	{
		class KRender
		{
		public:
			
			KRender(KE::SYSTEM::KWindow* _win) : _win(_win){}

			void run();

			const VkInstance GetVkInstance() { return _VkInstance; }
			
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

		private:

			void InitVulkan();
		
			void UpdateLoop();
			void CleanUp();
		
			void CreateVkInstance();
			void PickPhysicalDevice();
			void CreateWin32Surface();
			void CreateLogicalDevice();
			void CreateSwapChain();
			void CreateImageViews();
			void CreatePipeline();
			void CreateFramebuffers();

			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice);
			QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice);
		
			//PipeLine Helpers
			VkPipelineDynamicStateCreateInfo CreateDynaminceStateInfo(int DynamicStateCount, VkDynamicState* DynamicStateData);
			VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo();
			VkPipelineInputAssemblyStateCreateInfo CreateAssemblyInputStateInfo();
			VkPipelineViewportStateCreateInfo CreateViewPort();
			VkPipelineRasterizationStateCreateInfo CreateRasterizationState();
			VkPipelineColorBlendAttachmentState CreateColorBlendAttachmentState();
			VkRenderPassCreateInfo CreateRenderPassInfo();

			//All SwapChain helpers
			SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice _VkPhysicalDevice);
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
			bool CheckVaildationLayerSupport();
		
			//Helpers for required information
			std::vector<const char*> GetRequiredInstanceExtensions();
			std::vector<const char*> GetRequiredInstaceLayers();
			std::vector<const char*> GetRequiredDeviceExtensions();

		private:

			KE::SYSTEM::KWindow* _win;
			VkInstance _VkInstance;
			VkDevice _VkDevice;
			VkQueue _VkGraphicsQueue;
			VkQueue _VkPresentationQueue;
			VkSurfaceKHR _VkSurface;
			VkPhysicalDevice _VkPhysicalDevice;
			VkSwapchainKHR _VkSwapchain;
			VkFormat _VkSwapchainFormat;
			VkExtent2D _VkSwapchainExtent;
			VkPipelineLayout _VkPipelineLayout;

			std::vector<const char*> validationLayers;
			std::vector<const char*> InstanceExtensions;
			std::vector<const char*> deviceExtensions;

			std::vector<VkImage> SwapChainImages;
			std::vector<VkImageView> ImageViews;
			std::vector<VkDynamicState> DynamicStates;
			std::vector<VkFramebuffer> Framebuffers;
};
	}

}