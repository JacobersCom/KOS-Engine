#include <vulkan/vulkan.hpp>


namespace Kos
{

	/*
	This is where the magic happens
	*/
	class RenderCore
	{
	public:
		RenderCore();

		bool startup();

		void update();

		void shutdown();

	private:

		class KWindow* m_window = nullptr;
		class KDevice* m_device = nullptr;
		class KSwapchain* m_swapchain = nullptr;
		class KRenderpass* m_renderpass = nullptr;
		class KPipeline* m_pipeline = nullptr;

	};
}