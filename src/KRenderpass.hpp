#include <vulkan/vulkan.hpp>

namespace Kos
{
	class KRenderpass
	{
	public:

		KRenderpass(class KDevice* device) : m_device(device) {};
		~KRenderpass();

		VkRenderPassCreateInfo CreateRenderPassInfo(VkFormat format);
		void CreateFrameBuffers(std::vector<VkImageView> image_views, VkExtent2D extent);


		VkRenderPass GetRenderpassInfo() const { return m_renderpass; };

	private:

		VkRenderPass m_renderpass;

		std::vector<VkFramebuffer> m_frame_buffer;

	private:

		KDevice* m_device;
	};
}