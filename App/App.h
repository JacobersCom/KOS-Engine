#include "VulkanRenderer/KOSWindow.h"

class App
{
public:

	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	void run();

private:
	//KOSWindow resource initialization
	class KOSWindow window{ "TestWindow", WIDTH, HEIGHT };
};