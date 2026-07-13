#pragma once

#include <vulkan/vulkan.hpp>

namespace Kos
{

	/*
		Helper functions for creating the pipeline and creation of the pipeline itself
	*/
	class KPipeline
	{
	public:

		KPipeline() {};

		bool startup();
		bool shutdown();
		
		VkPipeline GetPipline() const { return m_pipeline; };
		VkPipelineLayout GetPipelineLayout() const { return m_pipeline_layout; };
		VkRect2D GetScissor() const { return m_scissor; };
		VkViewport GetViewport() const { return m_viewport; };

	private:

		void CreatePipeline(VkDevice device, VkRenderPass renderpass, VkExtent2D extents);


	private:

		
		VkPipeline m_pipeline;
		VkPipelineLayout m_pipeline_layout;

		VkViewport m_viewport;
		VkRect2D m_scissor;
	};
}