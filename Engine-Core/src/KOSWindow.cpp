#include "KOSWindow.hpp"



//ios
#include <iostream>
#include <errno.h>

#include "SDL3/SDL.h"


ENGINE::KOSWindow::KOSWindow(const char* WindowTitle, int width, int height)
	: windowTitle(WindowTitle), width(width), height(height)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        printf("Error: %s\n", strerror(errno));
        EXIT_FAILURE;
    }
}


bool ENGINE::KOSWindow::PollEvents()
{
    MSG message = {};
    //Peek message stop the window thread from waiting until it gets a message
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT)
        {
            windowState = false;
        }

        TranslateMessage(&message);
        DispatchMessage(&message);
    }


    return windowState = false;
}

void ENGINE::KOSWindow::startup()
{
    

    windowState = true;

	

    


   

    //Check if the render thread is working
   /* auto& sync = registry.ctx().get<RENDERER::RenderSync>();
    {
        std::lock_guard<std::mutex> lock(sync.state->lock);
        sync.state->surfaceReady = true;
    }*/
    //Let the render thread know that the surface is ready
    //sync.state->cv.notify_one();

    PollEvents();
}

