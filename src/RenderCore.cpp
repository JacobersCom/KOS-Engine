#include "RenderCore.hpp"

#include "KWindow.hpp"
#include "KDevice.hpp"
#include "KSwapchain.hpp"
#include "KRenderpass.hpp"
#include "KPipeline.hpp"


namespace Kos
{
	namespace 
	{

	}

	RenderCore::RenderCore()
	{
		m_window = new KWindow();
		m_device = new KDevice(m_window);
		m_swapchain = new KSwapchain(m_device, m_window);
		m_renderpass = new KRenderpass(m_device, m_swapchain);
		m_pipeline = new KPipeline(m_device, m_renderpass, m_swapchain);

	}

	bool RenderCore::startup()
	{
		m_window->Create("KOS-Engine", 800, 800);
		m_device->startup();
		m_swapchain->startup();
		m_renderpass->startup();
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