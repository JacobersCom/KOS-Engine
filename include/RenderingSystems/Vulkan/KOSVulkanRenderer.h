#include "KOSVulkanPrerequisites.h"

class VulkanBuffers; // May remove

/*
VulkanRenderer will is independed and can start up first
VulkanRenderer handles all the graphics operations and 
instance creation
*/
class VulkanRenderer {

public:

	VkInstance KInstance;
	bool KActive;


	void CreateInstance();

private:

	VulkanRenderer() {};
	~VulkanRenderer() {};

	/*
	Checks layers to ensure it is avabile on our system, 
	and returns that layer
	*/
	std::vector<const char*> GetRequiredDebuggingLayers();
};