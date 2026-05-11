#include "VulkanRenderer/KOSWindow.h"
#include "VulkanRenderer/KOSVulkanComponents.h"



//ios
#include <iostream>
#include <errno.h>
namespace
{
    LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            case WM_CLOSE:
            {
                DestroyWindow(handle);
                return 0;
            }
            case WM_SIZE:
            {
                break;
            }
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                //Create a device context for drawing using the GDI
                HDC hdc = BeginPaint(handle, &ps);

                //All painting happens here

                //rcPaint contains the client region on call of WM_PAINT
                HBRUSH windowColor = CreateSolidBrush(0x0000ff);

                FillRect(hdc, &ps.rcPaint, windowColor);

                EndPaint(handle, &ps);

                break;
            }
            default:
                return DefWindowProc(handle, message, wParam, lParam);
        }
    }
}


KOSWindow::KOSWindow(entt::registry& registry, const char* WindowTitle, int width, int height)
	: windowTitle(WindowTitle), width(width), height(height)
{
	CreateWin32(registry);
}


bool KOSWindow::PollEvents()
{
    MSG message = {};
    while (GetMessage(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return windowState = false;
}

void KOSWindow::CreateWin32(entt::registry& registry)
{
    entt::entity WindowEntity = registry.create();

    windowState = true;

	char className[] = "KOSWindowClass";
    HINSTANCE hInstance = GetModuleHandleA(nullptr);


    WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

        windowHandle = CreateWindowExA(
        0,
        className,
        windowTitle,
        WS_OVERLAPPEDWINDOW, //Window Style
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        NULL
        );

    if (NULL == windowHandle)
    {
        printf("Error: %s\n", strerror(errno));
        EXIT_FAILURE;
    }


    auto& surface = registry.emplace<RENDERER::Surface>(WindowEntity);
    surface.Win32Surface = VK_NULL_HANDLE;
    surface.WindowHandle = windowHandle;

    //Check if the render thread is working
    auto& sync = registry.view<RENDERER::RenderSync>();
    for (auto& thread : sync)
    {
        //if so set surface ready flag to true
        auto& renderThread = registry.get<RENDERER::RenderSync>(thread);
        {
            std::lock_guard<std::mutex> lock(renderThread.state->lock);
            renderThread.state->surfaceReady = true;
        }

        //Let the render thread know that the surface is ready
        renderThread.state->cv.notify_one();
    }

    ShowWindow(windowHandle, WM_SHOWWINDOW);     
}

