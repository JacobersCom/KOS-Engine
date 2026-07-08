#include <vulkan/vulkan.hpp>


namespace Kos
{

	/*
	This is where the magic happens
	*/
	class RenderCore
	{
	public:
		RenderCore(class KDevice& device) : m_device(device) {}

		void DrawFrame();
	private:

		KDevice& m_device;

	};
}