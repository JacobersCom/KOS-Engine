#include "KPipeline.hpp"

#include "KDevice.hpp"
#include "KRenderpass.hpp"
#include "KSwapchain.hpp"
#include "KModel.hpp"

#include <filesystem>
#include <fstream>

namespace Kos
{
	namespace 
	{
		std::vector<char> LoadShader(std::filesystem::path& _FileName)
		{
			//Starts reading at the end of the file
			std::ifstream file(_FileName, std::ios::ate | std::ios::binary);

			if (!file.is_open())
			{
				throw std::runtime_error("Failed to load shader files");

			}

			//Returns the files size in bytes
			auto fileSize = file.tellg();
			std::vector<char> buffer(fileSize);

			//Reset the postion pointer to the beginnering of the file
			file.seekg(0);
			//Read shader data into vector
			file.read(buffer.data(), fileSize);

			file.close();
			return buffer;
		}

		/*
		*/
		VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice device)
		{
			VkShaderModuleCreateInfo ShaderModuleCreateInfo{};
			ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			ShaderModuleCreateInfo.codeSize = code.size();
			ShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

			VkShaderModule _VkShaderModule;

			VkResult result = vkCreateShaderModule(device, &ShaderModuleCreateInfo, nullptr, &_VkShaderModule);
			if (result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create Shader Module");
			}

			return _VkShaderModule;
		}

		VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo(int vertex_attribute_count, int vertex_binding_count)
		{
			auto binding_descriptions = KModel::Vertex::getBindingDescriptions();
			 auto attribute_descriptions = KModel::Vertex::getAttributeDescriptions();

			VkPipelineVertexInputStateCreateInfo VertexStateInfo{};
			VertexStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			VertexStateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
			VertexStateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
			VertexStateInfo.pVertexAttributeDescriptions = attribute_descriptions.data();
			VertexStateInfo.pVertexBindingDescriptions = binding_descriptions.data();
			return VertexStateInfo;
		}

		VkPipelineInputAssemblyStateCreateInfo CreateAssemblyInputStateInfo(VkPrimitiveTopology topology)
		{
			VkPipelineInputAssemblyStateCreateInfo AssemblyStateInfo{};
			AssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			AssemblyStateInfo.topology = topology;
			AssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

			return AssemblyStateInfo;
		}

		VkViewport CreateViewportAndScissor(VkViewport& viewport, VkRect2D& scissor, VkExtent2D extent)
		{
			VkViewport viewport_info{};
			viewport_info.x = 0.0f;
			viewport_info.y = 0.0f;
			viewport_info.width = static_cast<float>(extent.width);
			viewport_info.height = static_cast<float>(extent.height);
			viewport_info.minDepth = 0.0f;
			viewport_info.maxDepth = 1.0f;

			return viewport_info;
			
			VkRect2D _VkScissor{};

			_VkScissor.offset = { 0, 0 };
			_VkScissor.extent = extent;
		}

		VkPipelineViewportStateCreateInfo CreateViewPortStateInfo(VkViewport& viewport, VkRect2D& scissor,
			uint32_t viewport_count, uint32_t scissor_count)
		{
			VkPipelineViewportStateCreateInfo ViewPortInfo{};
			ViewPortInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			ViewPortInfo.viewportCount = viewport_count;
			ViewPortInfo.pViewports = &viewport;
			ViewPortInfo.scissorCount = scissor_count;
			ViewPortInfo.pScissors = &scissor;

			return ViewPortInfo;
		}

		VkPipelineRasterizationStateCreateInfo CreateRasterizationState()
		{
			VkPipelineRasterizationStateCreateInfo RasterStateInfo{};
			RasterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			RasterStateInfo.depthClampEnable = VK_FALSE; // Using this requires enabling VkPipelineRasterizationDepthClipStateCreateInfoEXT within the pipeline
			RasterStateInfo.rasterizerDiscardEnable = VK_FALSE; // if true it stops everything from reaching the frame buffer
			RasterStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
			RasterStateInfo.lineWidth = 1.f; //Anything thicker than 1.f requries a gpu feature

			//Determines the type of face culling to use
			RasterStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
			RasterStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

			RasterStateInfo.depthBiasEnable = VK_FALSE;
			RasterStateInfo.depthBiasConstantFactor = 0.0f;
			RasterStateInfo.depthBiasClamp = 0.0f;
			RasterStateInfo.depthBiasSlopeFactor = 0.0f;

			return RasterStateInfo;
		}

		VkPipelineColorBlendAttachmentState CreateColorBlendInfo()
		{
			VkPipelineColorBlendAttachmentState ColorblendAttachmentInfo{};

			ColorblendAttachmentInfo.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT |
				VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT |
				VK_COLOR_COMPONENT_A_BIT;

			return ColorblendAttachmentInfo;
		}

		VkPipelineColorBlendStateCreateInfo CreatePipelineColorBlendStateInfo(
			VkPipelineColorBlendAttachmentState& ColorBlendAttachmentInfo)
		{

			VkPipelineColorBlendStateCreateInfo ColorblendingStateInfo{};
			ColorblendingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			ColorblendingStateInfo.logicOpEnable = VK_FALSE;
			ColorblendingStateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
			ColorblendingStateInfo.attachmentCount = 1;
			ColorblendingStateInfo.pAttachments = &ColorBlendAttachmentInfo;
			ColorblendingStateInfo.blendConstants[0] = 0.0f; // Optional
			ColorblendingStateInfo.blendConstants[1] = 0.0f; // Optional
			ColorblendingStateInfo.blendConstants[2] = 0.0f; // Optional
			ColorblendingStateInfo.blendConstants[3] = 0.0f; // Optional

			return  ColorblendingStateInfo;
		}

		VkPipelineDynamicStateCreateInfo CreateDynaminceStateInfo(int DynamicStateCount, VkDynamicState* DynamicStateData)
		{
			VkPipelineDynamicStateCreateInfo DynamicStateInfo{};
			DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			DynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(DynamicStateCount);
			DynamicStateInfo.pDynamicStates = DynamicStateData;

			return DynamicStateInfo;
		}

		VkPipelineMultisampleStateCreateInfo CreatePipelineMultisampleStateInfo()
		{
			VkPipelineMultisampleStateCreateInfo MultiSampleInfo{};
			MultiSampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			MultiSampleInfo.sampleShadingEnable = VK_FALSE; //allows the pixel shader to be evaulated for each sample within a pixel instead of one fragment
			MultiSampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; //Number of samples used in rasterzation
			MultiSampleInfo.minSampleShading = 1.0f; //The minmum number of sample shading
			MultiSampleInfo.alphaToCoverageEnable = VK_FALSE; //Creates a temp value based off the color of the fragments first output
			MultiSampleInfo.alphaToOneEnable = VK_FALSE; //Controls wither the aphla value of the first color is replaced on one

			return MultiSampleInfo;
		}

	}

	void KPipeline::startup()
	{
		CreatePipeline(m_device->GetDevice(), m_renderpass->GetRenderpass(), m_swapchain->GetExtent());
	}

	void KPipeline::shutdown()
	{

	}

	void KPipeline::CreatePipeline(VkDevice device, VkRenderPass renderpass, VkExtent2D extents)
	{
		auto VertShaderCode = LoadShader(std::filesystem::path(KENGINE_SHADER_DIR) / "VertShader.spv");
		auto PixelShaderCode = LoadShader(std::filesystem::path(KENGINE_SHADER_DIR) / "FragShader.spv");

		VkShaderModule VertModule = CreateShaderModule(VertShaderCode, device);
		VkShaderModule PixelModule = CreateShaderModule(PixelShaderCode, device);

		VkPipelineShaderStageCreateInfo VertexStage{};
		VertexStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		VertexStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		VertexStage.module = VertModule;
		VertexStage.pName = "main";

		VkPipelineShaderStageCreateInfo PixelStage{};
		PixelStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		PixelStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		PixelStage.module = PixelModule;
		PixelStage.pName = "main";

		VkPipelineShaderStageCreateInfo shader_stages[] = { VertexStage, PixelStage };

		auto binding_descriptions = KModel::Vertex::getBindingDescriptions();
		auto attribute_descriptions = KModel::Vertex::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertex_input{};
		vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
		vertex_input.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
		vertex_input.pVertexAttributeDescriptions = attribute_descriptions.data();
		vertex_input.pVertexBindingDescriptions = binding_descriptions.data();

		VkPipelineInputAssemblyStateCreateInfo assembly_input = CreateAssemblyInputStateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		CreateViewportAndScissor(m_viewport, m_scissor, extents);

		VkPipelineViewportStateCreateInfo viewport_info = CreateViewPortStateInfo(m_viewport, m_scissor, 1, 1);

		VkPipelineRasterizationStateCreateInfo rasterization_state = CreateRasterizationState();

		VkPipelineColorBlendAttachmentState ColorBlendAttachment = CreateColorBlendInfo();

		VkPipelineColorBlendStateCreateInfo color_blend_state = CreatePipelineColorBlendStateInfo(ColorBlendAttachment);

		std::vector<VkDynamicState> States = { VK_DYNAMIC_STATE_SCISSOR,VK_DYNAMIC_STATE_VIEWPORT};

		VkPipelineDynamicStateCreateInfo dynamic_state = CreateDynaminceStateInfo(States.size(), States.data());

		VkPipelineMultisampleStateCreateInfo multisample_state = CreatePipelineMultisampleStateInfo();

		VkPipelineLayoutCreateInfo pipe_line_layout_Info{};
		pipe_line_layout_Info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipe_line_layout_Info.setLayoutCount = 0;
		pipe_line_layout_Info.pSetLayouts = nullptr;
		pipe_line_layout_Info.pushConstantRangeCount = 0;
		pipe_line_layout_Info.pPushConstantRanges = nullptr;

		VkResult result = vkCreatePipelineLayout(device, &pipe_line_layout_Info, nullptr, &m_pipeline_layout);

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create GraphicsPipeLine");
		}

		VkGraphicsPipelineCreateInfo pipe_line_info{};
		pipe_line_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipe_line_info.stageCount = 2;
		pipe_line_info.pStages = shader_stages;
		pipe_line_info.pVertexInputState = &vertex_input;
		pipe_line_info.pInputAssemblyState = &assembly_input;
		pipe_line_info.pViewportState = &viewport_info;
		pipe_line_info.pRasterizationState = &rasterization_state;
		pipe_line_info.pColorBlendState = &color_blend_state;
		pipe_line_info.pMultisampleState = &multisample_state;
		pipe_line_info.pDynamicState = &dynamic_state;
		pipe_line_info.layout = m_pipeline_layout;
		pipe_line_info.renderPass = renderpass;
		pipe_line_info.subpass = 0; //Index of subpass where this pipeline will be used
		pipe_line_info.basePipelineHandle = VK_NULL_HANDLE; //Ref to another pipeline
		pipe_line_info.basePipelineIndex = 0; //Index of pipeline

		result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipe_line_info, nullptr, &m_pipeline);

		if (result != VK_SUCCESS) {

			throw std::runtime_error("Failed to create GraphicsPipeLine");
		}

		//Clean up
		vkDestroyShaderModule(device, VertModule, nullptr);
		vkDestroyShaderModule(device, PixelModule, nullptr);
	}

	/*
	binds the pipline, viewport and scissor to the GPU pipeline
	*/
	void KPipeline::bind(VkCommandBuffer buffer)
	{
		vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
		//Because the viewport and scissor is dynmic so they must be set in the command buffer before drawing
		vkCmdSetViewport(buffer, 0, 1, &m_viewport);
		vkCmdSetScissor(buffer, 0, 1, &m_scissor);
	}
}