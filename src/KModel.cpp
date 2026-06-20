#include "KModel.hpp"

void Kos::KModel::createVertexBuffer(const std::vector<Vertex>& vertices)
{
	VkBufferCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //Allows buffers to be shared between graphics queues
	info.size = sizeof(vertices[0]) * vertices.size();

	if (vkCreateBuffer(kDevice, &info, nullptr, &vertexBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}

	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(kDevice, vertexBuffer, &memRequirements);
}

uint32_t Kos::KModel::findMemeoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags)
{
	//Query the available memeory types on the physical device
	VkPhysicalDeviceMemoryProperties phyMemProperties{};
	vkGetPhysicalDeviceMemoryProperties(kPhyDevice, &phyMemProperties);

	//Find the right memory type
	for (uint32_t a = 0; a < phyMemProperties.memoryTypeCount; a++)
	{
		if (typeFilter & (1 << a))
		{
			return a;
		}
	}
	throw std::runtime_error("Failed to find memory type");



}
