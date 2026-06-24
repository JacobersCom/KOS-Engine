
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