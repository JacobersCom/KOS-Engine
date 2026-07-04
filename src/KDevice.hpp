
//defines
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // WIN

//includes
#include "pch.hpp"
#include "vulkan/vulkan.hpp"

//ios
#include <memory>
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

		std::optional<uint32_t>GraphicsFamily;
		std::optional<uint32_t>PresentFamily;


		bool isComplete()
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	/*
	* Handles the startup of the vulkan instance
	*/
	class KDevice
	{
	public:
		KDevice(class KWindow& window) : k_window(window), k_instance(0), k_surface(0), k_physical_device(0){}

		bool startup();
		bool shutdown();

		//Vulkan start up functions
		void CreateInstance();
		void CreateSurface();
		void FindUsersGPU();
		void CreateLogicDevice();
		VkRenderPassCreateInfo CreateRenderPassInfo(VkFormat format);
		void CreateFrameBuffers(std::vector<VkImageView> image_views, VkExtent2D extent);

		//Helper functions
		bool RateDeviceSuitable(VkPhysicalDevice phy_device, VkSurfaceKHR surface);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice phy_device, VkSurfaceKHR surface);

		//Member variables
		VkInstance k_instance;
		VkSurfaceKHR k_surface;
		VkPhysicalDevice k_physical_device;
		VkDevice k_logical_device;
		VkRenderPass m_renderpass;
		std::vector<VkFramebuffer> arr_frame_buffers;

	
	private:

		KWindow& k_window;

	};
}