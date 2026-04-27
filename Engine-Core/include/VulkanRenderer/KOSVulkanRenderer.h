#include "vulkan/vulkan.h"


class VulkanBuffers; // May remove

/*
VulkanRenderer will is independed and can start up first
VulkanRenderer handles all the graphics operations and 
instance creation
*/
class KOSVulkanRenderer {

public:

	VkInstance KInstance;
	bool KActive;


	void CreateInstance();

private:

	KOSVulkanRenderer() {};
	~KOSVulkanRenderer() {};

	/*
	Checks layers to ensure it is avabile on our system, 
	and returns that layer
	*/
	std::vector<const char*> GetRequiredDebuggingLayers();

	/*
	Checks if the users GPU has the required support for features of the engine
	*/
	bool IsGPUSupported(VkPhysicalDevice PhysicalDevice);
};