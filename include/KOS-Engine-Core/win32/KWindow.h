#pragma once

#include <Windows.h>
#include <ShObjIdl.h>
#include <shlobj_core.h>
#include <string>

#define ID_BTN_OK 1


namespace KE
{
	namespace SYSTEM
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
			void OpenDialogBox();
			void EventDispatcher();

			const HWND GetWindowHandle() { return WindowHandle; }
			const HINSTANCE GetWindowInstance() { return PtrLoader; }

		private:

			static LRESULT CALLBACK WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam);
		
			void CreateWin32Window(WindowDesc& desc);
			void CreateWin32Button();
		

		private:

			//ptr to where the DLL is held
			HINSTANCE PtrLoader;

			//Handle to the window
			HWND WindowHandle;
		

			WindowDesc _WindowDesc;

		};
	}

};
