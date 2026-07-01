#include "KPipeline.hpp"

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
	}

	void KPipeline::CreatePipeline(VkDevice device, VkRenderPass renderpass)
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

		VkPipelineShaderStageCreateInfo ShaderStages[] = { VertexStage, PixelStage };

		VkPipelineVertexInputStateCreateInfo VertexInput = CreateVertexInputStateInfo();

		VkPipelineInputAssemblyStateCreateInfo AssemblyInput = CreateAssemblyInputStateInfo();

		_VkViewport = CreateViewportInfo();
		_VkScissor = CreateScissorInfo();

		VkPipelineViewportStateCreateInfo ViewPortInfo = CreateViewPortStateInfo(_VkViewport, _VkScissor, 1, 1);

		VkPipelineRasterizationStateCreateInfo RasterizationInfo = CreateRasterizationState();

		VkPipelineColorBlendAttachmentState ColorBlendAttachment = CreateColorBlendInfo();

		VkPipelineColorBlendStateCreateInfo ColorBlendInfo = CreatePipelineColorBlendStateInfo(ColorBlendAttachment);

		std::vector<VkDynamicState> States = CreateDynamicStates();
		VkPipelineDynamicStateCreateInfo DynamicStateInfo = CreateDynaminceStateInfo(States.size(), States.data());

		VkPipelineMultisampleStateCreateInfo MultiSampleInfo = CreatePipelineMultisampleStateInfo();

		VkPipelineLayoutCreateInfo PipelinelayoutInfo{};
		PipelinelayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		PipelinelayoutInfo.setLayoutCount = 0;
		PipelinelayoutInfo.pSetLayouts = nullptr;
		PipelinelayoutInfo.pushConstantRangeCount = 0;
		PipelinelayoutInfo.pPushConstantRanges = nullptr;

		VkResult result = vkCreatePipelineLayout(device, &PipelinelayoutInfo, nullptr, &m_pipeline_layout);

		if (result != VK_SUCCESS)
		{
			printf("Failed to create pipeline layout");
		}

		VkGraphicsPipelineCreateInfo PipelineInfo{};
		PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		PipelineInfo.stageCount = 2;
		PipelineInfo.pStages = ShaderStages;
		PipelineInfo.pVertexInputState = &VertexInput;
		PipelineInfo.pInputAssemblyState = &AssemblyInput;
		PipelineInfo.pViewportState = &ViewPortInfo;
		PipelineInfo.pRasterizationState = &RasterizationInfo;
		PipelineInfo.pColorBlendState = &ColorBlendInfo;
		PipelineInfo.pMultisampleState = &MultiSampleInfo;
		PipelineInfo.pDynamicState = &DynamicStateInfo;
		PipelineInfo.layout = m_pipeline_layout;
		PipelineInfo.renderPass = renderpass;
		PipelineInfo.subpass = 0; //Index of subpass where this pipeline will be used
		PipelineInfo.basePipelineHandle = VK_NULL_HANDLE; //Ref to another pipeline
		PipelineInfo.basePipelineIndex = 0; //Index of pipeline

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
			&PipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {

			throw std::runtime_error("Failed to create GraphicsPipeLine");
		}

		//Clean up
		vkDestroyShaderModule(device, VertModule, nullptr);
		vkDestroyShaderModule(device, PixelModule, nullptr);
	}
}