

/*
KOSVulkanWindow depends on KOSVulkanRenderer to be enabled first
KOSVulkanWindow handles everything that is presented to the window
*/
class KOSVulkanWindow {
public:


	KOSVulkanWindow(const char* WindowTitle, int width, int height);

	/*
	Used to created a surface for the rendering system
	*/

private:

	void KOSCreateWindow();

	const char* windowTitle;
	int width, height;

};