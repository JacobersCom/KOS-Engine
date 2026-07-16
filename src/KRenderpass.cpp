#include "KRenderpass.hpp"

#include "KDevice.hpp"
#include "KSwapchain.hpp"

namespace Kos
{
	namespace 
	{

	}


	void KRenderpass::startup()
	{
		CreateRenderPassInfo(m_swapchain->GetFormat());
		CreateFrameBuffers(m_swapchain->GetImageViews(), m_swapchain->GetExtent());
	}

	/*
	RenderPassInfo is need to discarible how many depth, color buffers there will be and how
	many samples to use for each of them and how there contents should be handle during the
	rendering process

	A RenderPass allows for operations to be performed on a image to improve the quaitily of the final image
	*/
	VkRenderPassCreateInfo KRenderpass::CreateRenderPassInfo(VkFormat format)
	{
		//The description of an color attachment
		VkAttachmentDescription color_desc{};
		color_desc.format = format; //Format of the image view that will be used
		color_desc.samples = VK_SAMPLE_COUNT_1_BIT; //Each pixel's depth, coverage, and stencil will be tested once

		//These value will describle how the contents color and depth are treated before and after pass
		color_desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //Values will become const at the start
		color_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //Write contents to memory to read later

		//These value will describle how the contents of the stencil components will be treated before and after pass
		color_desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; //Do not save the content within the render area
		color_desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //Content is not needed after rendering

		//The layout of the image from beginning to end of pass
		color_desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //Image layout unknown and not saved.
		color_desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //Images in the swapchain

		//Simply a reference to the ColorAttachment above
		VkAttachmentReference color_ref{};
		color_ref.attachment = 0; //The index is found in the shader code COLOR0
		color_ref.layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL; //Gives better performance

		//Subpasses allow for different effects to be grouped togther in a renderpass to reorder the operations to save on memory and give better performance
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //Tells Vulkan this is a graphics subpass
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_ref;

		//SubpassDependencys manage the transition of the image layouts
		VkSubpassDependency subpass_deps{};

		subpass_deps.srcSubpass = VK_SUBPASS_EXTERNAL; //Ensures the subpass begins before vkCmdBeginRenderpass
		subpass_deps.dstSubpass = 0;

		subpass_deps.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;//The subpass will wait to till the image is done reading from the swapchain
		subpass_deps.srcAccessMask = 0;

		//Where the dst commands will have access to memory
		subpass_deps.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpass_deps.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		//Linking it all togther
		VkRenderPassCreateInfo renderpass_info{};
		renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderpass_info.attachmentCount = 1;
		renderpass_info.pAttachments = &color_desc;
		renderpass_info.subpassCount = 1;
		renderpass_info.pSubpasses = &subpass;
		renderpass_info.dependencyCount = 1;
		renderpass_info.pDependencies = &subpass_deps;

		VkResult result = vkCreateRenderPass(m_device->GetDevice(), &renderpass_info, nullptr, &m_renderpass);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create render pass");
		}

		return renderpass_info;
	}

	/*
	Creates a FrameBuffer for all the images that correspond to
	the retrieved image at drawing time.

	Framebuffers are references to VkImageView objects that
	represent attachments
	*/
	void KRenderpass::CreateFrameBuffers(std::vector<VkImageView> image_views, VkExtent2D extent)
	{

		m_frame_buffer.resize(image_views.size());

		//Iterate through the Imageviews to create a framebuffer form them
		for (int i = 0; i < image_views.size(); i++)
		{
			//Lists of attachements for each _VkSawpchainImageview
			VkImageView attachments[] = {
				image_views[i]
			};

			VkFramebufferCreateInfo FramebufferInfo{};
			FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferInfo.renderPass = m_renderpass;
			FramebufferInfo.attachmentCount = 1;
			FramebufferInfo.pAttachments = attachments;
			FramebufferInfo.height = extent.height;
			FramebufferInfo.width = extent.width;
			FramebufferInfo.layers = 1; //the total sides of an image

			if (vkCreateFramebuffer(m_device->GetDevice(), &FramebufferInfo, nullptr, &m_frame_buffer[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create Framebuffer for Swapchain Image view");
			}
		}

	}

	void KRenderpass::BeginRenderPass(VkCommandBuffer buffer)
	{
		VkRenderPassBeginInfo RenderPassBeginInfo{};
		RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassBeginInfo.renderPass = m_renderpass;
		RenderPassBeginInfo.framebuffer = *m_frame_buffer.data(); //The framebuffer and the attachements 
		//Affected area of the render pass
		RenderPassBeginInfo.renderArea.offset = { 0,0 };
		RenderPassBeginInfo.renderArea.extent = m_swapchain->GetExtent();

		VkClearValue ClearValues = { {{0.0f, 0.0f, 0.0f, 1.0f}} }; //Clear color used for VK_ATTACHMENT_LOAD_OP_CLEAR
		RenderPassBeginInfo.clearValueCount = 1;
		RenderPassBeginInfo.pClearValues = &ClearValues;

		//The commands being recorded will be stored here and VK_SUBPASS_CONTENTS_INLINE means the commands
		//will be embedded in the primary command buffer
		vkCmdBeginRenderPass(buffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}
}