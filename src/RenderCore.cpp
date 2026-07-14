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
			{{0.0f, 0.0f,}}
		};

		m_model = std::make_unique<KModel>(m_device, vertices);
	}

	/*
	Creates a command pool on the CPU to record commands to be passed into the GPU queue
	*/
	void RenderCore::CreateCommandPool(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool command_pool)
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

	void RenderCore::CreatePrimaryCommandBuffer()
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

	void RenderCore::SyncObjects()
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

	/*
	TODO: Turn Commandbuffer into a vector to enable muilt images being render
	*/
	void RenderCore::RecordCommandBuffers(VkCommandBuffer, uint32_t image_index)
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
		RenderPassBeginInfo.renderArea.extent =

			VkClearValue ClearValues = { {{0.0f, 0.0f, 0.0f, 1.0f}} }; //Clear color used for VK_ATTACHMENT_LOAD_OP_CLEAR
		RenderPassBeginInfo.clearValueCount = 1;
		RenderPassBeginInfo.pClearValues = &ClearValues;

		//The commands being recorded will be stored here and VK_SUBPASS_CONTENTS_INLINE means the commands
		//will be embedded in the primary command buffer
		vkCmdBeginRenderPass(m_command_buffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		//Make a function for this in the pipeline object
		vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipline());

		//Because the viewport and scissor is dynmic so they must be set in the command buffer before drawing
		
		//Same with these
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


}