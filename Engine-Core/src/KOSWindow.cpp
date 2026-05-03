#include "VulkanRenderer/KOSWindow.h"
#include "VulkanRenderer/KOSVulkanComponents.h"

#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#define VK_USE_PLATFORM_WIN32_KHR
#include "Windows.h"
#endif


//ios
#include <iostream>
#include <errno.h>

KOSWindow::KOSWindow(entt::registry& registry, const char* WindowTitle, int width, int height)
	: windowTitle(WindowTitle), width(width), height(height)
{
	CreateWin32(registry);
}

LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_QUIT:
    {
        DestroyWindow(handle);
    }
    break;
    }

    return DefWindowProc(handle, message, wParam, lParam);
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
    entt::entity Display = registry.create();


    windowState = true;

	char className[] = "KOSWindowClass";

    WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
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
       GetModuleHandle(nullptr),
        NULL
        );

    if (NULL == windowHandle)
    {
        printf("Error: %s\n", strerror(errno));
        EXIT_FAILURE;
    }

    registry.emplace<RENDERER::Surface>(Display, windowHandle);

    ShowWindow(windowHandle, 1);     
}

