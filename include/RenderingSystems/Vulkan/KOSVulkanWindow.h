#include "KOSVulkanPrerequisites.h"

class KOSVulkanRenderer;

/*
KOSVulkanWindow depends on KOSVulkanRenderer to be enabled first
KOSVulkanWindow handles everything that is presented to the window
*/
class KOSVulkanWindow {
public:

	const char* KWindowTitle;
	int KWidth, KHeight;
	bool KActive;

	KOSVulkanWindow(const char* WindowTitle, int width, int height);

	/*
	Used to created a surface for the rendering system
	*/
	void CreateSurface(size_t windowhandle);

private:

};