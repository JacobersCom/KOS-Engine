
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
		KDevice(class KWindow& window) : m_window(window), m_instance(0), m_surface(0), m_physical_device(0){}

		//Vulkan start up functions
		void CreateInstance();
		void CreateSurface();
		void FindUsersGPU();

		//Helper functions
		bool RateDeviceSuitable(VkPhysicalDevice phy_device, VkSurfaceKHR surface);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice phy_device, VkSurfaceKHR surface);

		//Member variables
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physical_device;
	
	private:

		//Pointers to classes
		KWindow& m_window;

	};
}