#include "KWindow.h"
#include "KReturn.h"

#include <windows.h>

namespace KE
{
	namespace SYSTEM
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

		//Whatever thread calls this function will own the COM model init in the function
		KE::KReturn KWindow::OpenDialogBox()
		{
			//Disable dynamic data exchange
			HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			if (SUCCEEDED(result))
			{
				IFileOpenDialog* pFileOpen;


				result = CoCreateInstance( //Creates and fefault init a single object of the class associated with a specified CLSID
					CLSID_FileOpenDialog, //Data and code associated with the object
					NULL, // Wont be created as part of an aggreate
					CLSCTX_ALL, //Context in which the newly create will run
					IID_IFileOpenDialog, //Interface used to talk to the created object
					reinterpret_cast<void**>(&pFileOpen)
					);

				if (SUCCEEDED(result))
				{
					//Show open dialog box
					result = pFileOpen->Show(NULL);

					if (SUCCEEDED(result))
					{
						//The file the user selected
						IShellItem* pItem;

						result = pFileOpen->GetResult(&pItem);

						if (SUCCEEDED(result))
						{

							PWSTR LocalAppDataPath;
							result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &LocalAppDataPath);

							if (FAILED(result))
							{
								return KReturn::K_FAILURE;
							}

							//Create the folder path and create a new dir for it


						}
						pItem->Release();
					}	
				pFileOpen->Release();
				}
			CoUninitialize();
			}
			return KE::KReturn::K_SUCCESS;
		}
	
		KReturn KWindow::CreateWin32Window(WindowDesc& Desc)
		{
			PtrLoader = GetModuleHandle(NULL);
			WNDCLASS wc = {};
			wc.hInstance = PtrLoader;
			wc.lpszClassName = reinterpret_cast<LPCWSTR>(Desc.ClassName);
			wc.lpfnWndProc = KE::SYSTEM::KWindow::WindowProc;

			RegisterClass(&wc);

			WindowHandle = CreateWindowEx(0, Desc.ClassName, Desc.WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT
				, CW_USEDEFAULT, Desc.WindowWidth, Desc.WindowHeight, NULL, NULL, PtrLoader, NULL);

			if (WindowHandle == nullptr) return KReturn::K_WINDOW_CREATION_FAILED;

			ShowWindow(WindowHandle, SW_SHOW);

			return KReturn::K_WINDOW_CREATION_SUCCESS;
		}

		//Add parameters to this to make it a helper
		KE::KReturn KWindow::CreateWin32Button()
		{
			HWND hwnd = CreateWindowEx(
				0,
				L"BUTTON",  // Predefined class; Unicode assumed 
				L"Import",      // Button text 
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
			case WM_COMMAND:
			{
			//This is only the case for WM_COMMAND
			
				//Low bits contain the ID of the button
				int ControllerID = LOWORD(wParam);
				//Hit order contain the macro called
				int Notification = HIWORD(wParam);

				if (Notification == BN_CLICKED)
				{

					KE::SYSTEM::KWindow* pThis = reinterpret_cast<KE::SYSTEM::KWindow*>(GetWindowLongPtr(
						Window, 
						GWLP_USERDATA //retrieves the data use by the application to create the window
					));
				
					switch (ControllerID)
					{
						case ID_BTN_OK:
							pThis->OpenDialogBox();
							break;
					}

					delete pThis;
				}
				break;
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
	}
}


