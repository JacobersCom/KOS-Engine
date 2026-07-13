#include <vulkan/vulkan.hpp>

namespace Kos
{
	class KRenderpass
	{
	public:

		KRenderpass(class KDevice* device) : m_device(device) {};
		~KRenderpass();

		VkRenderPassCreateInfo CreateRenderPassInfo(VkFormat format);

		VkRenderPass GetRenderpassInfo() const { return m_renderpass; };

	private:

		VkRenderPass m_renderpass;

	private:

		KDevice* m_device;
	};
}