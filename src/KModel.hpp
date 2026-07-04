#pragma once

#include "KVector3.hpp"
#include "pch.hpp"

#include "vulkan/vulkan.h"

namespace Kos
{
	class KDevice;

	class KModel
	{
	public:

		struct Vertex
		{
			Kos::math::KVector3 position;

			std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		KModel(KDevice& device, const std::vector<Vertex>& vertices);
		~KModel();

		KModel(const KModel&) = delete;
		KModel& operator=(const KModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		KDevice& k_device;

		VkBuffer k_vertex_buffer;
		VkDeviceMemory k_vertex_buffer_memory;
		
		uint32_t vertexCount;
	};
}