#include "KModel.hpp"
#include "KDevice.hpp"

#include <cassert>

namespace Kos
{
	Kos::KModel::KModel(KDevice* device, const std::vector<Vertex>& vertices) : m_device(device)
	{
		createVertexBuffers(vertices);
	}

	KModel::~KModel()
	{
		vkDestroyBuffer(m_device->GetDevice(), k_vertex_buffer, nullptr);
		vkFreeMemory(m_device->GetDevice(), k_vertex_buffer_memory, nullptr);
	}

	void KModel::bind(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void KModel::draw(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { k_vertex_buffer };
		VkDeviceSize offset[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offset);
	}

	void KModel::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3!");
		VkDeviceSize buffer_size = sizeof(vertices[0]) * vertexCount;

		m_device->CreateBuffer(buffer_size,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // allows the host(cpu) to write to the device(gpu) memory and keeps the GPU and CPU memory consiston with one another
			k_vertex_buffer, k_vertex_buffer_memory);

		void* data;
		//creates a region of host memory mapped to device memory
		// and sets data to point to the mapped memory range
		vkMapMemory(m_device->GetDevice(), k_vertex_buffer_memory, 0, buffer_size, 0, &data);
		//memcpy sends the data from the CPU to the GPU because of VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		//Without it we would need to use flush to send the data over, but we dont need to because
		//the memory is mapped to the GPU from the CPU
		memcpy(data, vertices.data(), static_cast<size_t>(buffer_size));
		vkUnmapMemory(m_device->GetDevice(), k_vertex_buffer_memory);
	}

	std::vector<VkVertexInputBindingDescription> Kos::KModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> binding_description(1);
		binding_description[0].binding = 0;
		binding_description[0].stride = sizeof(Vertex);
		binding_description[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return binding_description;
	}

	std::vector<VkVertexInputAttributeDescription> Kos::KModel::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attribute_description(1);
		attribute_description[0].binding = 0;
		attribute_description[0].location = 0;
		attribute_description[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_description[0].offset = 0;

		return attribute_description;
	}
}