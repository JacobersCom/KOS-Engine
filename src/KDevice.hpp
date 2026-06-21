#include "pch.hpp"
#include "vulkan/vulkan.hpp"


namespace Kos
{
	/*
	* Handles the startup of the vulkan instance
	*/
	class KDevice
	{
	public:
		KDevice() : mInstance(0){}

		void CreateInstance();

	private:
		bool CheckVaildationLayersSupport(std::vector<const char*> layers);

	private:
		VkInstance mInstance;
	};
}