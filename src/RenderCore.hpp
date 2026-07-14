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
		void CreateCommandPool(VkCommandPool command_pool);
		void CreatePrimaryCommandBuffer(VkCommandPool command_pool, VkCommandBuffer buffer);
		void SyncObjects();
		void RecordCommandBuffers(VkCommandBuffer, uint32_t image_index);
		

	private:

		std::unique_ptr<class KWindow> m_window;
		std::unique_ptr<class KDevice> m_device;
		std::unique_ptr<class KSwapchain> m_swapchain;
		std::unique_ptr<class KRenderpass> m_renderpass;
		std::unique_ptr<class KPipeline> m_pipeline;
		std::unique_ptr<class KModel> m_model;

		VkCommandPool m_cmd_pool;
		VkCommandBuffer m_cmd_buffer;

		VkSemaphore m_image_available;
		VkSemaphore m_render_finished;
		VkFence m_frames_in_flight;


	};
}