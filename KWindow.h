#pragma once

#include <Windows.h>


//Namespace holds data for making the window
namespace KE::SYSTEM
{

	struct WindowDesc
	{
		const WCHAR* ClassName;
		const WCHAR* WindowTitle;
		int WindowWidth;
		int WindowHeight;

	};
	
	//Class holds functions and private memebers for class
	class KWindow
	{
	public:

		void Create();
		void SetWindowDesc(const WCHAR* WindowClassName, const WCHAR* WindowTitle,
			const int WindowWidth, const int WindowHeight);
		void GetFrameBufferSize(HWND WindowHandle, int& Width, int& Height);
		
		KE::KReturn EventDispatcher();

		const HWND GetWindowHandle() { return WindowHandle; }
		const HINSTANCE GetWindowInstance() { return PtrLoader; }

	private:

		KReturn CreateWin32Window(WindowDesc& desc);

	private:

		//ptr to where the DLL is held
		HINSTANCE PtrLoader;

		//Handle to the window
		HWND WindowHandle;

		WindowDesc _WindowDesc;
	};

};

LRESULT CALLBACK WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam);