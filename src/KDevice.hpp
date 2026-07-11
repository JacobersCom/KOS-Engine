
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
		
		KDevice(class KWindow& window) : m_window(window){}
		~KDevice();

		bool startup();
		bool shutdown();
		void DrawFrame();

		//Accessors
		VkInstance GetInstance() const { return m_instance; };
		VkSurfaceKHR GetSurface() const { return m_surface; };
		VkPhysicalDevice GetPhysicalDevice() const { return m_physical_device; };
		VkDevice GetDevice() const { return m_device; };
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice phy_device, VkSurfaceKHR surface);
		
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags useage, VkMemoryPropertyFlags properties
			,VkBuffer& buffer, VkDeviceMemory& buffer_memeory);


	private:

		//Core functions
		void CreateInstance();
		void CreateSurface();
		void FindUsersGPU();
		
		void CreateLogicDevice();
		VkRenderPassCreateInfo CreateRenderPassInfo(VkFormat format);
		
		void CreateFrameBuffers(std::vector<VkImageView> image_views, VkExtent2D extent);
		
		void CreateCommandPool(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool command_pool);
		void CreatePrimaryCommandBuffer();
		
		void SyncObjects();
		void RecordCommandBuffers(VkCommandBuffer, uint32_t image_index);

		//Helper functions
		bool RateDeviceSuitable(VkPhysicalDevice phy_device, VkSurfaceKHR surface);

	private:

		KWindow& m_window;

		VkInstance m_instance = 0;
		VkSurfaceKHR m_surface = 0;
		VkPhysicalDevice m_physical_device = 0;
		VkDevice m_device = 0;
	};
}