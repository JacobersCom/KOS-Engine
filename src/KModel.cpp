#include "KModel.hpp"
#include "KDevice.hpp"

#include <cassert>

namespace Kos
{
	Kos::KModel::KModel(KDevice& device, const std::vector<Vertex>& vertices) : k_device(device)
	{
		createVertexBuffers(vertices);
	}

	KModel::~KModel()
	{
		vkDestroyBuffer(k_device.k_logical_device, k_vertex_buffer, nullptr);
		vkFreeMemory(k_device.k_logical_device, k_vertex_buffer_memory, nullptr);
	}

	void KModel::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3!");
		VkDeviceSize buffer_size = sizeof(vertices[0]) * vertexCount;
	}
}