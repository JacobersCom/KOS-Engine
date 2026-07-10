
//defines
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // WIN

//includes
#include "pch.hpp"
#include "vulkan/vulkan.hpp"

//ios
#include <optional>
#include <set>


namespace Kos
{

	/*
	* Stores a unsigned value of the different queue families
	*
	* Mainly used in ChooseUserGPU to ensure that the end users has the needed QueueFamilys on his GPU
	*/
	struct QueueFamilyIndices {

		std::optional<uint32_t>m_graphics_family;
		std::optional<uint32_t>m_present_family;


		bool isComplete()
		{
			return m_graphics_family.has_value() && m_present_family.has_value();
		}
	};

	/*
	* Handles the startup of the vulkan instance
	*/
	class KDevice
	{
	public:
		
		KDevice(class KWindow& window, class KSwapchain& swapchain, class KPipeline& pipeline) : m_window(window),m_swapchain(swapchain), m_pipeline(pipeline){}



		bool startup();
		bool shutdown();
		void DrawFrame();

		//Accessors
		VkInstance GetInstance() const { return m_instance; };
		VkSurfaceKHR GetSurface() const { return m_surface; };
		VkPhysicalDevice GetPhysicalDevice() const { return m_physical_device; };
		VkDevice GetDevice() const { return m_device; };


	private:

		//Core functions
		void CreateInstance();
		void CreateSurface();
		void FindUsersGPU();
		
		void CreateLogicDevice();
		VkRenderPassCreateInfo CreateRenderPassInfo(VkFormat format);
		
		void CreateFrameBuffers(std::vector<VkImageView> image_views, VkExtent2D extent);
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags useage, VkMemoryPropertyFlags properties
			,VkBuffer& buffer, VkDeviceMemory& buffer_memeory);
		
		void CreateCommandPool(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool command_pool);
		void CreatePrimaryCommandBuffer();
		
		void SyncObjects();
		void RecordCommandBuffers(VkCommandBuffer, uint32_t image_index);

		//Helper functions
		bool RateDeviceSuitable(VkPhysicalDevice phy_device, VkSurfaceKHR surface);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice phy_device, VkSurfaceKHR surface);

	private:

		KWindow& m_window;
		KSwapchain& m_swapchain;
		KPipeline& m_pipeline;

		VkInstance m_instance = 0;
		VkSurfaceKHR m_surface = 0;
		VkPhysicalDevice m_physical_device = 0;
		VkDevice m_device = 0;
		VkRenderPass m_renderpass = 0;
		
		VkCommandPool m_command_pool = 0;
		VkCommandBuffer m_command_buffer = 0;
		
		VkSemaphore m_image_available = 0; 
		VkSemaphore m_render_finished = 0;
		VkFence m_frames_in_flight = 0;

		std::vector<VkFramebuffer> m_frame_buffers;
	};
}