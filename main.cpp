
#include "KWindow.h"
#include "KRender.h"
#include "KVector3.h"
#include "KMat4.h"

#include <iostream>

int main()
{
	KE::SYSTEM::KWindow _win;
	KE::RENDERER::KRender _render(_win);
	KE::RENDERER::QueueFamilyIndices fm;
	KE::RENDERER::SwapChainSupportDetails sd;


	_win.SetWindowDesc(L"KOS", L"KOS Engine", 500, 500);
	_win.Create();
	_render.run();
	std::cout << sizeof(sd);
	_win.EventDispatcher();

	
}