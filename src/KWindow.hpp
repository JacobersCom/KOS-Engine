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

		KWindow() : m_window_handle(0),  m_win_instance(0){}
		~KWindow() = default;

		//Abstruction for CreateWin32
		void Create(const char* title, int w, int h);
		void GetFrameBufferSize(int& Width, int& Height);

		//Update loop
		void ProcessMessages();

		//Accessors
		const HWND GetWindowHandle() { return m_window_handle; }
		const HINSTANCE GetWindowInstance() { return m_win_instance; }

	private:

		HINSTANCE m_win_instance;
		HWND m_window_handle;
	};

};
