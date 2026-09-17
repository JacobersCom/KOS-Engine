#pragma once

#include "NonCopyable.h"
#include "VkCommon.h"



class KRenderer : public NonCopyable
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