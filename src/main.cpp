
#include "RenderCore.hpp"
#include "KLog.hpp"

#include <iostream>


//Fixed error logging to only use KLog
int main()
{
	Kos::RenderCore m_render_core;
	m_render_core.startup();
	
	m_render_core.update();




	/*while (true)
	{
		_win.ProcessMessages();
		
		if (!IsWindow(_win.GetWindowHandle()))
		{
			break;
		}
	}*/


}