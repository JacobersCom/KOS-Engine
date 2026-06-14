#pragma once

#include <vulkan/vulkan.h>

namespace Kos
{
	class KSawpchain
	{
		KSawpchain();
		~KSawpchain();

		bool GetSawpchainDetails(VkPhysicalDevice device);

	private:



		struct SawpchainDetails
		{

		}
	};
}
