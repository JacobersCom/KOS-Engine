#include <vulkan/vulkan.hpp>

#include "pch.hpp"

namespace Kos
{

	/*
	This is where the magic happens
	*/
	class RenderCore
	{
	public:
		RenderCore();
		~RenderCore();


		bool startup();

		void update();

		void shutdown();

	private:

		void LoadModel();
		void CreateCommandPool(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool command_pool);
		void CreatePrimaryCommandBuffer();
		void SyncObjects();
		void RecordCommandBuffers(VkCommandBuffer, uint32_t image_index);
		

	private:

		std::unique_ptr<class KWindow> m_window;
		std::unique_ptr<class KDevice> m_device;
		std::unique_ptr<class KSwapchain> m_swapchain;
		std::unique_ptr<class KRenderpass> m_renderpass;
		std::unique_ptr<class KPipeline> m_pipeline;
		std::unique_ptr<class KModel> m_model;

	};
}