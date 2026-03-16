#pragma once

#include <Windows.h>
#include <ShObjIdl.h>

#define ID_BTN_OK 1

namespace KE
{
	enum class KReturn : int;
}

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
		KE::KReturn GetFrameBufferSize(HWND WindowHandle, int& Width, int& Height);
		KE::KReturn OpenDialogBox();
		KE::KReturn EventDispatcher();

		const HWND GetWindowHandle() { return WindowHandle; }
		const HINSTANCE GetWindowInstance() { return PtrLoader; }

	private:

		static LRESULT CALLBACK WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam);
		
		KE::KReturn CreateWin32Window(WindowDesc& desc);
		KE::KReturn CreateWin32Button();
		

	private:

		//ptr to where the DLL is held
		HINSTANCE PtrLoader;

		//Handle to the window
		HWND WindowHandle;
		

		WindowDesc _WindowDesc;

	};

};
