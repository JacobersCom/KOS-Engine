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

		bool startup();
		void shutdown();

	private:

		KDevice& m_device;

	};
}