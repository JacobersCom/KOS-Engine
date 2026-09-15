#pragma once

#include "NonCopyable.h"
#include "Common.h"


#include <vulkan/vulkan.h>

class KRenderer : public NonCopyable
{
public:
	
	KRenderer() = default;
	
	~KRenderer();

	bool InitRenderer();

private:


	bool is_initialized;

	VkInstance		 k_instance;
	VkPhysicalDevice k_physical;
	VkDevice		 k_device;
};