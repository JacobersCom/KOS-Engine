#include "RenderCore.hpp"


namespace Kos
{
	namespace 
	{

	}

	RenderCore::RenderCore()
	{
		m_window = std::make_unique<KWindow>();
		device = std::make_unique<KDevice>(m_window.get());
		m_swapchain = std::make_unique<KSwapchain>(device.get(), m_window.get());
		m_renderpass = std::make_unique<KRenderpass>(device.get(), m_swapchain.get());
		m_pipeline = std::make_unique<KPipeline>(device.get(), m_renderpass.get(), m_swapchain.get());
	}

	bool RenderCore::startup()
	{
		m_window->Create("KOS-Engine", 800, 800);
		device->startup();
		m_swapchain->startup();
		m_renderpass->startup();
		m_pipeline->startup();
		LoadModel();
		CreateCommandPool(cmd_pool);
		CreatePrimaryCommandBuffer(cmd_pool, cmd_buffer);
		m_swapchain->SyncDeviceWork();

		return true;
	}
	

	void RenderCore::update()
	{
		while (true)
		{
			if (!IsWindow(m_window->GetWindowHandle()))
				break;

			m_window->ProcessMessages();
			DrawFrame();
		}
		vkDeviceWaitIdle(device->GetDevice());
	}

	void RenderCore::LoadModel()
	{
		std::vector<KModel::Vertex> vertices
		{
			{{0.0f, 0.5f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}}

		};

		m_model = std::make_unique<KModel>(device.get(), vertices);
	}

	/*
	Creates a command pool on the CPU to record commands to be passed into the GPU queue
	*/
	void RenderCore::CreateCommandPool(VkCommandPool& command_pool)
	{
		QueueFamilyIndices Indices = device->GetQueueFamilyIndices();

		VkCommandPoolCreateInfo CommandPoolInfo{};
		CommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		//allows the command buffer to record all the commands but if a reset happens it will have to rerecord all the commands
		CommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CommandPoolInfo.queueFamilyIndex = Indices.m_graphics_family.value();

		if (vkCreateCommandPool(device->GetDevice(), &CommandPoolInfo, nullptr, &command_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool");
		}

	}

	void RenderCore::CreatePrimaryCommandBuffer(VkCommandPool command_pool, VkCommandBuffer& buffer)
	{
		VkCommandBufferAllocateInfo AllocateInfo{};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = command_pool;
		AllocateInfo.commandBufferCount = 1;

		//Can be submitted to queue but not to another command buffer
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(device->GetDevice(), &AllocateInfo, &buffer))
		{
			throw std::runtime_error("Failed to create primary command buffer");
		}
	}


	void RenderCore::RecordCommandBuffers(VkCommandBuffer buffer)
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

	void RenderCore::DrawFrame()
	{
		uint32_t image_index;

		RecordCommandBuffers(cmd_buffer);

		VkResult result = m_swapchain->AcquireNextImage(&image_index);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to acquire next swapchain image");
		}

		result = m_swapchain->SubmitCommandBuffers(cmd_buffer, &image_index);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit commands to buffer");
		}
	}
}