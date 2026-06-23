#include "KWindow.hpp"
#include <string>
#include <assert.h>
#include <windows.h>

namespace Kos
{
	namespace
	{
		LRESULT CALLBACK WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}
			case WM_CLOSE:
			{
				DestroyWindow(Window);
				break;
			}
			}
			return DefWindowProc(Window, message, wParam, lParam);
		}

	}

	void KWindow::Create(const char* title, int w, int h)
	{
		m_win_instance = GetModuleHandle(NULL);
		WNDCLASS wc = {};
		wc.hInstance = m_win_instance;
		wc.lpszClassName = "KWindow";
		wc.lpfnWndProc = WindowProc;

		RegisterClass(&wc);

		m_window_handle = CreateWindowEx(0,
			"KWindow",
			title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			w,
			h,
			NULL,
			NULL,
			m_win_instance,
			NULL);

		if (m_window_handle != nullptr)
		{
			ShowWindow(m_window_handle, SW_SHOW);
		}
	}

	void Kos::KWindow::ProcessMessages()  
	{
		MSG msg;
		if (PeekMessage(&msg, m_window_handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		unsigned int frameCount = 0;
		++frameCount;
		unsigned int framesPasted = frameCount;
		unsigned int  preCount = GetTickCount64();

		//Updated every second
		if (GetTickCount64() - preCount > 1000)
		{
			char buffer[256];
			sprintf_s(buffer, "%s FPS: %d", "KOS", static_cast<int> (frameCount - framesPasted));
			SetWindowText(m_window_handle, buffer);
			framesPasted = frameCount;
			preCount = GetTickCount64();
		}

	}

	/*
	GetFrameBufferSize is used when handling the vulkan swapchain.

	this function has DPI support for all VkImages so they are scale appropriately
	*/
	void KWindow::GetFrameBufferSize(HWND handle, int& w, int& h)
	{
		RECT rect{};
		GetClientRect(handle, &rect);

		int dpiX = GetDpiForWindow(handle);
		float dpiScale = dpiX / USER_DEFAULT_SCREEN_DPI;

		int logicalW = rect.left - rect.right;
		int logicalH = rect.top - rect.bottom;

		float scaleX = dpiX / dpiScale;
		float scaleY = (float)GetDpiForWindow(handle) / dpiScale;

		w = (int)(logicalW * scaleX);
		h = (int)(logicalH * scaleY);
	}

}


