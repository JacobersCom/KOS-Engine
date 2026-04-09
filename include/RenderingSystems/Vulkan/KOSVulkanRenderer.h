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


	void Init();

private:

	VulkanRenderer() {};
	~VulkanRenderer() {};


};