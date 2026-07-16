#pragma once

#include <vulkan/vulkan.hpp>

#include "KWindow.hpp"
#include "KDevice.hpp"
#include "KSwapchain.hpp"
#include "KRenderpass.hpp"
#include "KPipeline.hpp"
#include "KModel.hpp"

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
		//~RenderCore();


		bool startup();

		void update();

		void shutdown();

	private:

		void LoadModel();
		void CreateCommandPool(VkCommandPool command_pool);
		void CreatePrimaryCommandBuffer(VkCommandPool command_pool, VkCommandBuffer buffer);
		void RecordCommandBuffers(VkCommandBuffer, uint32_t image_index);
		void DrawFrame();
		

	private:

		std::unique_ptr<KWindow> m_window;
		std::unique_ptr<KDevice> device;
		std::unique_ptr<KSwapchain> m_swapchain;
		std::unique_ptr<KRenderpass> m_renderpass;
		std::unique_ptr<KPipeline> m_pipeline;
		std::unique_ptr<KModel> m_model;

		VkCommandPool m_cmd_pool;
		VkCommandBuffer m_cmd_buffer;
	};
}