#include "RenderCore.hpp"

#include "KWindow.hpp"
#include "KDevice.hpp"
#include "KSwapchain.hpp"
#include "KRenderpass.hpp"
#include "KPipeline.hpp"
#include "KModel.hpp"
#include "KVector3.hpp"


namespace Kos
{
	namespace 
	{

	}

	RenderCore::RenderCore()
	{
		m_window = std::make_unique<KWindow>();
		m_device = std::make_unique<KDevice>(m_window);
		m_swapchain = std::make_unique<KSwapchain>(m_device, m_window);
		m_renderpass = std::make_unique<KRenderpass>(m_device, m_swapchain);
		m_pipeline = std::make_unique<KPipeline>(m_device,m_renderpass, m_swapchain);
	}

	bool RenderCore::startup()
	{
		m_window->Create("KOS-Engine", 800, 800);
		m_device->startup();
		m_swapchain->startup();
		m_renderpass->startup();
		m_pipeline->startup();

		return true;
	}
	

	void RenderCore::update()
	{
		while (true)
		{
			m_window->ProcessMessages();
			if (!IsWindow(m_window->GetWindowHandle()))
			{
				break;
			}
			m_device->DrawFrame();
		}
	}

	void RenderCore::LoadModel()
	{
		std::vector<KModel::Vertex> vertices
		{
			{{0.0f, 0.5f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}}

		};

		m_model = std::make_unique<KModel>(m_device, vertices);
	}

	/*
	Creates a command pool on the CPU to record commands to be passed into the GPU queue
	*/
	void RenderCore::CreateCommandPool(VkCommandPool command_pool)
	{
		QueueFamilyIndices Indices = m_device->GetQueueFamilyIndices();

		VkCommandPoolCreateInfo CommandPoolInfo{};
		CommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		//allows the command buffer to record all the commands but if a reset happens it will have to rerecord all the commands
		CommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CommandPoolInfo.queueFamilyIndex = Indices.m_graphics_family.value();

		if (vkCreateCommandPool(m_device->GetDevice(), &CommandPoolInfo, nullptr, &command_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool");
		}

	}

	void RenderCore::CreatePrimaryCommandBuffer(VkCommandPool command_pool, VkCommandBuffer buffer)
	{
		VkCommandBufferAllocateInfo AllocateInfo{};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = command_pool;
		AllocateInfo.commandBufferCount = 1;

		//Can be submitted to queue but not to another command buffer
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(m_device->GetDevice(), &AllocateInfo, &buffer))
		{
			throw std::runtime_error("Failed to create primary command buffer");
		}
	}


	void RenderCore::SyncObjects()
	{
		VkSemaphoreCreateInfo SemaphoreInfo{};
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo FenceInfo{};
		FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; //fences start signed

		if (vkCreateSemaphore(m_device->GetDevice(), &SemaphoreInfo, nullptr, &m_image_available) != VK_SUCCESS
			|| vkCreateSemaphore(m_device->GetDevice(), &SemaphoreInfo, nullptr, &m_render_finished) != VK_SUCCESS
			|| vkCreateFence(m_device->GetDevice(), &FenceInfo, nullptr, &m_frames_in_flight) != VK_SUCCESS
			)
		{
			throw std::runtime_error("Failed to create semaphores and fence!");
		}
	}

	void RenderCore::RecordCommandBuffers(VkCommandBuffer buffer, uint32_t image_index)
	{
		VkCommandBufferBeginInfo BeginInfo{};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(buffer, &BeginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to write to commandbuffer");
		}

		m_renderpass->BeginRenderPass(buffer);

		m_pipeline->bind(buffer);

		//Now we can draw
		m_model->bind(buffer);
		m_model->draw(buffer);

		vkCmdEndRenderPass(buffer);

		if (vkEndCommandBuffer(buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record to command buffer");
		}
	}


}