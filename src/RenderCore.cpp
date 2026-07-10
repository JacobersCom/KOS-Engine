#include "RenderCore.hpp"
#include "KDevice.hpp"
#include "KSwapchain.hpp"
#include "KPipeline.hpp"
#include "KWindow.hpp"


namespace Kos
{
	namespace 
	{

	}

	RenderCore::RenderCore()
	{
		m_window = new KWindow();
		m_device = new KDevice(m_window, m_swapchain, m_pipeline);
		m_swapchain = new KSwapchain(m_device, m_window);
		m_pipeline = new KPipeline(m_device);

	}

	bool RenderCore::startup()
	{
		m_window->Create("KOS-Engine", 800, 800);
		m_device->startup();
		m_swapchain->startup(m_device->GetPhysicalDevice(), m_device->GetSurface());
		m_pipeline->startup();

		return true;
	}
	
	void RenderCore::update()
	{
		while (true)
		{
			m_window->ProcessMessages();
			if (!IsWindow(m_window->GetWindowHandle()))
			{
				break;
			}
			m_device->DrawFrame();
		}
	}
}