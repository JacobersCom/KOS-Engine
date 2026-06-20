#pragma once

#include "vulkan/vulkan.h"
#include "KVector3.hpp"

#include <vector>

namespace Kos
{
	class KModel
	{
	public:

		struct Vertex
		{
			Kos::Vec3D position;

			std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		KModel(VkDevice& device, const std::vector<Vertex>& vertices);
		~KModel();

		KModel(const KModel&) = delete;
		KModel& operator=(const KModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		
		void createVertexBuffer(const std::vector<Vertex>& vertices);

		VkDevice& Kdevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}