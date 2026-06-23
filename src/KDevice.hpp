
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
	* Stores all the needed details of the swapchain
	* 
	* Mainly used in ChooseUserGPU to ensure that the end users has the needed swapchain cabailities
	*/
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR SurfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> ImageFormats;
		std::vector<VkPresentModeKHR> PresentMode;
	};

	/*
	* Handles the startup of the vulkan instance
	*/
	class KDevice
	{
	public:
		KDevice() : m_instance(0), m_surface(0), m_physical_device(0){}

		//Vulkan start up functions
		void CreateInstance();
		void CreateSurface();
		void FindUsersGPU();

		//Member variables
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physical_device;

		//Pointers to classes
		std::unique_ptr<class KWindow> m_window;

	};
}