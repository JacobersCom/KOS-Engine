


class VulkanBuffers; // May remove

/*
VulkanRenderer will is independed and can start up first
VulkanRenderer handles all the graphics operations and 
instance creation
*/
class KOSVulkanRenderer {

public:


	KOSVulkanRenderer(std::string ApplicationName);
	~KOSVulkanRenderer();

	KOSVulkanRenderer(const KOSVulkanRenderer&) = delete;
	KOSVulkanRenderer& operator=(const KOSVulkanRenderer) = delete;

private:
	
	void InitVulkan();

	std::string ApplicationName;

};