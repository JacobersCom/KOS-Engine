#pragma once

#include <Common.h>
#include <Gateware.h>
#include <vulkan/vulkan.h>

class KRenderer
{
public:
	
	KRenderer() = default;
	
	~KRenderer();
	
	void CreateInstance(const char* app_name, U32 verison);
	void CreateSurface(void* window_handle);

private:


	bool			 is_initialized;

	VkInstance		 k_instance;
	VkSurfaceKHR	 k_surface;
	VkPhysicalDevice k_physical;
	VkDevice		 k_device;
};