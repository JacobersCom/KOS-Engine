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
		/*
		* Finds a suitable type of memory by iterating the memory types and finding which bit field is set to 1
		* And
		* the suitable heap and property for the choosen memeory type
		* @param
		* typeFilter - used to specify a bit field for a suitable memory type
		*/
		uint32_t findMemeoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);
		
		VkPhysicalDevice kPhyDevice;
		VkDevice& kDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}