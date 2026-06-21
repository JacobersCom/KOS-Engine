#pragma once

#include <Windows.h>
#include "KObject.hpp"
#include <ShObjIdl.h>
#include <shlobj_core.h>
#include <string>

#define ID_BTN_OK 1


namespace Kos
{
	
	//Class holds functions and private memebers for class
	class KWindow : public KObject
	{
	public:

		KWindow() : handle(0),  instance(0){}
		~KWindow() = default;

		//Abstruction for CreateWin32
		void Create(const char* title, int w, int h);
		void GetFrameBufferSize(HWND WindowHandle, int& Width, int& Height);

		void ProcessMessages();

		const HWND GetWindowHandle() { return handle; }
		const HINSTANCE GetWindowInstance() { return instance; }

	private:

		static LRESULT CALLBACK WindowProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam);
		
	private:

		//ptr to where the DLL is held
		HINSTANCE instance;

		//Handle to the window
		HWND handle;
	};

};
