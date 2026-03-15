#include "KWindow.h"
#include "KReturn.h"

#include <windows.h>

namespace KE::SYSTEM
{
	void KWindow::Create()
	{
		//Creates the window
		CreateWin32Window(_WindowDesc);
		CreateWin32Button();
	}

	void KWindow::SetWindowDesc(const WCHAR* WindowClassName, const WCHAR* WindowTitle, 
		const int WindowWidth, const int WindowHeight)
	{
		KE::SYSTEM::WindowDesc desc = {};

		 desc.ClassName = WindowClassName;
		desc.WindowTitle = WindowTitle;
		desc.WindowWidth = WindowWidth;
		desc.WindowHeight = WindowHeight;
		
		_WindowDesc = desc;

	
	}

	KE::KReturn KWindow::GetFrameBufferSize(HWND WindowHandle, int& Width, int& Height)
	{
		RECT ClientRect;
		GetClientRect(WindowHandle, &ClientRect);

		//Top left screen coord
		POINT p0{ ClientRect.left, ClientRect.top };
		//Bottom right screen coord
		POINT p1{ ClientRect.right, ClientRect.bottom };

		using WinFp = BOOL(WINAPI*)(HWND, LPPOINT);

		HMODULE User32 = GetModuleHandleW(L"User32.dll");

		//Returns address of function else returns nullptr.  
		auto logicalToPhysicalPM = (WinFp)GetProcAddress(User32, "LogicalToPhysicalPointForPerMonitorDPI");
		

		if (logicalToPhysicalPM)
		{
			logicalToPhysicalPM(WindowHandle, &p0);
			logicalToPhysicalPM(WindowHandle, &p1);
		}
		else
		{
			return KE::KReturn::K_FAILURE;
		}

		Width = (int)(p1.x - p0.x);
		Height = (int)(p1.y - p0.y);

		return KE::KReturn::K_SUCCESS;
	}

	KE::KReturn KWindow::OpenDialogBox()
	{
		
	}
	
	KReturn KWindow::CreateWin32Window(WindowDesc& Desc)
	{
		PtrLoader = GetModuleHandle(NULL);
		WNDCLASS wc = {};
		wc.hInstance = PtrLoader;
		wc.lpszClassName = reinterpret_cast<LPCWSTR>(Desc.ClassName);
		wc.lpfnWndProc = ::WindowProc;

		RegisterClass(&wc);

		WindowHandle = CreateWindowEx(0, Desc.ClassName, Desc.WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT
			, CW_USEDEFAULT, Desc.WindowWidth, Desc.WindowHeight, NULL, NULL, PtrLoader, NULL);

		if (WindowHandle == nullptr) return KReturn::K_WINDOW_CREATION_FAILED;

		ShowWindow(WindowHandle, SW_SHOW);

		return KReturn::K_WINDOW_CREATION_SUCCESS;
	}

	KE::KReturn KWindow::CreateWin32Button()
	{
		HWND hwnd = CreateWindowEx(
			0,
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"OK",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			10,         // x position 
			10,         // y position 
			100,        // Button width
			100,        // Button height
			WindowHandle,     // Parent window
			(HMENU)ID_BTN_OK,       // No menu.
			(HINSTANCE)GetWindowLongPtr(WindowHandle, GWLP_HINSTANCE),
			NULL);      // Pointer not needed.

		return KE::KReturn::K_SUCCESS;
	}

	KReturn KWindow::EventDispatcher()
	{
		MSG msg = {};

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return KReturn::K_PROGRAM_CLOSED;
	}


}


//Static Functions
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
		case WM_COMMAND:
		{
		//This is only the case for WM_COMMAND
			
			//Low bits contain the ID of the button
			int ControllerID = LOWORD(wParam);
			//Hit order contain the macro called
			int Notification = HIWORD(wParam);

			if (Notification == BN_CLICKED)
			{
				switch (ControllerID)
				{
					case ID_BTN_OK:
						
						break;
				}
			}
		}
		case WM_SIZE:
		{
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			int width = ClientRect.right - ClientRect.left;
			int height = ClientRect.bottom - ClientRect.top;
			break;
		}
		case WM_PAINT:
		{

			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			RECT ClientRECT;
			GetClientRect(Window, &ClientRECT);
			//EndPaint(hwnd, &Paint);
			break;
		}
	}

	return DefWindowProc(Window, message, wParam, lParam);
}