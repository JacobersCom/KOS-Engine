#include "Window.hpp"
//ios
#include <iostream>
#include <errno.h>


Kos::Window::Window(const char* WindowTitle, int width, int height)
	: windowTitle(WindowTitle), width(width), height(height)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        printf("Error: %s\n", strerror(errno));
        EXIT_FAILURE;
    }
}

bool Kos::Window::startup()
{
    windowHandle = SDL_CreateWindow(windowTitle, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
    if (!windowHandle)
    {
        SDL_Log("Failed to create window. Error: %s", SDL_GetError());
        return false;
    }
    
    SDL_Log("Window system initalzation!");
    return true;

    //Check if the render thread is working
   /* auto& sync = registry.ctx().get<RENDERER::RenderSync>();
    {
        std::lock_guard<std::mutex> lock(sync.state->lock);
        sync.state->surfaceReady = true;
    }*/
    //Let the render thread know that the surface is ready
    //sync.state->cv.notify_one();

}

