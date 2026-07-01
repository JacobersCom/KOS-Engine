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

		KPipeline(class KDevice& device) : k_device(device){};

		bool startup();
		bool shutdown();

		void CreatePipeline(VkDevice device, VkRenderPass renderpass, VkExtent2D extents);

		VkPipeline m_pipeline;
		VkPipelineLayout m_pipeline_layout;

		VkViewport m_viewport;
		VkRect2D m_scissor;

	private:

		class KDevice& k_device;
	};
}