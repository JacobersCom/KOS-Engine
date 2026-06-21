#include "KWindow.hpp"
#include <string>
#include <assert.h>
#include <windows.h>

namespace Kos
{
	void KWindow::Create(const char* title, int w, int h)
	{
		instance = GetModuleHandle(NULL);
		WNDCLASS wc = {};
		wc.hInstance = instance;
		wc.lpszClassName = "KWindow";
		wc.lpfnWndProc = Kos::KWindow::WindowProc;

		RegisterClass(&wc);

		handle = CreateWindowEx(0,
			"KWindow",
			title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			w,
			h,
			NULL,
			NULL,
			instance,
			NULL);

		if (handle != nullptr)
		{
			ShowWindow(handle, SW_SHOW);
		}
	}

	void Kos::KWindow::ProcessMessages()  
	{
		MSG msg;
		if (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
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
			SetWindowText(handle, buffer);
			framesPasted = frameCount;
			preCount = GetTickCount64();
		}

	}

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

	LRESULT CALLBACK KWindow::WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam)
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


