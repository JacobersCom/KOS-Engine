#pragma once

#include "NonCopyable.h"
#include "VkCommon.h"



class KRenderer : public NonCopyable
{
public:
	
	KRenderer() = default;
	
	~KRenderer();
	
	bool CreateInstance(const char* app_name, U32 verison);

private:


	bool			 is_initialized;

	VkInstance		 k_instance;
	VkPhysicalDevice k_physical;
	VkDevice		 k_device;
};