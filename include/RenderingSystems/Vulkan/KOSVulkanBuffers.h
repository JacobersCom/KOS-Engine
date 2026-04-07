#pragma once

#pragma region Defines
#define VK_USE_PLATFORM_WIN32_KHR
const int FRAMES_IN_FLIGHT = 2;

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
			bool InitVulkan();
			void UpdateLoop();
			void DrawFrame();
			void CleanUp();

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

		
		
			void CreateVkInstance();
			void PickPhysicalDevice();
			void CreateWin32Surface();
			void CreateLogicalDevice();
			void CreateSwapChain();
			void CreateImageViews();
			void CreatePipeline();

			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _VkPhysicalDevice);
			QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice _VkPhysicalDevice);
		
			//PipeLine Helpers
			VkPipelineDynamicStateCreateInfo CreateDynaminceStateInfo(int DynamicStateCount, VkDynamicState* DynamicStateData);
			VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo();
			VkPipelineInputAssemblyStateCreateInfo CreateAssemblyInputStateInfo();

			VkViewport CreateViewportInfo();
			VkRect2D CreateScissorInfo();
			VkPipelineViewportStateCreateInfo CreateViewPortStateInfo(VkViewport& _VkViewport, VkRect2D& _VkScissor,
				uint32_t ViewportCount, uint32_t ScissorCount);
			
			VkPipelineRasterizationStateCreateInfo CreateRasterizationState();

			VkPipelineColorBlendAttachmentState CreateColorBlendInfo();
			VkPipelineColorBlendStateCreateInfo CreatePipelineColorBlendStateInfo(
				VkPipelineColorBlendAttachmentState& ColorBlendState);

			std::vector<VkDynamicState> CreateDynamicStates(); 
			VkPipelineMultisampleStateCreateInfo CreatePipelineMultisampleStateInfo();
			
			void CreateFramebuffers();

			VkRenderPassCreateInfo CreateRenderPassInfo();

			void CreateCommandPool();
			void CreateCommandBuffers();

			/*
			Writes commands to the CommandBuffer we want to run
			*/
			void RecordCommandBuffer(VkCommandBuffer _VkCommandBuffer, uint32_t ImageIndex);

			/*
			Used to create sempahores
			*/
			void CreateSyncObjects();

			
			//All SwapChain helpers
			SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice _VkPhysicalDevice);
			VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR> formats);
			VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities);


			//All Shader helpers
			//Takes in the a spv compiled shader
			std::vector<char> LoadShader(std::filesystem::path& _FileName);
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
			VkViewport _VkViewport;
			VkRect2D _VkScissor;
			VkRenderPass _VkRenderPass;

			VkCommandPool _VkCommandPool;
			std::vector<VkCommandBuffer> _VkCommandBuffer;
			
			VkPipelineLayout _VkPipelineLayout;
			VkPipeline _VkPipeline;

			std::vector<VkSemaphore> imageAvailableSemaphore;
			std::vector<VkSemaphore> renderFinishedSemaphore;
			std::vector<VkFence> inFlightFence;

			std::vector<const char*> _VkValidationLayers;
			std::vector<const char*> _VkInstanceExtensions;
			std::vector<const char*> _VkDeviceExtensions;

			std::vector<VkImage> _VkSwapchainImages;
			std::vector<VkImageView> _VkSwapchainImageViews;
			std::vector<VkFramebuffer> _VkFramebuffers;
};
	}

}