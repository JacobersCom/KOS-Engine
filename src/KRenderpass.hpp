#include <vulkan/vulkan.hpp>

namespace Kos
{
	class KRenderpass
	{
	public:

		KRenderpass(class KDevice* device, class KSwapchain* swapchain) : m_device(device), m_swapchain(swapchain) {};

		void startup();
		void shutdown();

		void BeginRenderPass(VkCommandBuffer buffer);

		VkRenderPass GetRenderpass() const { return m_renderpass; };

	private:


		VkRenderPassCreateInfo CreateRenderPassInfo(VkFormat format);
		void CreateFrameBuffers(std::vector<VkImageView> image_views, VkExtent2D extent);

		VkRenderPass m_renderpass;

		std::vector<VkFramebuffer> m_frame_buffer;

	private:

		KDevice* m_device;
		KSwapchain* m_swapchain;

	};
}