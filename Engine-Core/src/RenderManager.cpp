#include "VulkanRenderer/RenderManager.h"
#include "VulkanRenderer/KOSVulkanComponents.h"


#include <chrono>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

namespace RENDERER
{
	bool InitializeRenderer(entt::registry& registry, entt::entity rendererEntity);
	void RenderFrame(entt::registry& registry, entt::entity rendererEntity);
	void CleanupRenderer(entt::registry& registry, entt::entity rendererEntity);
}


RenderManager::RenderManager(entt::registry& registry)
	: registryRef(&registry)
{

	entt::entity syncEntity = registry.create();
	auto& sync = registry.emplace<RENDERER::RenderSync>(syncEntity);
	auto surfaceView = registry.view<RENDERER::Surface>();
	//Is there a entity with a valid surface state?
	sync.state->surfaceReady = surfaceView.begin() != surfaceView.end();

	/* Creates a new thread that creates the render manager
	and ensures registry is a & */
	renderThread = std::thread(&RenderManager::StartRenderThread, this,std::ref(registry));
}

void RenderManager::RequestShutDown()
{

}

void RenderManager::StartRenderThread(entt::registry& registry)
{
	auto syncView = registry.view<RENDERER::RenderSync>();
	//No entity has the RenderSync component
	if (syncView.begin() == syncView.end())
	{
		return;
	}

	//else get the component from the first entity
	auto& sync = registry.get<RENDERER::RenderSync>(*syncView.begin());

	{
		//Locks thread
		std::unique_lock<std::mutex> lock(sync.state->sharedLock);
		
		//Unlock thread if either the surface is ready or shutdown was called
		sync.state->cv.wait(lock, [&sync] {

			return sync.state->surfaceReady || sync.state->shutdown;
		});
		
		if (sync.state->shutdown)
		{
			return;
		}
	}

	//Create new rendererEntity
	entt::entity rendererEntity = registry.create();
	registry.emplace<RENDERER::VulkanData>(rendererEntity);


	try
	{
		if (!RENDERER::InitializeRenderer(registry, rendererEntity))
		{
			return;
		}

		//This may cause errors down the line
		while (true)
		{
			{
				//If out of this scop unlock he thread
				std::lock_quard<std::mutex> lock(sync.state->sharedLock);
				
				//Check for program shutdown
				if (sync.state->shutdown)
				{
					break;
				}
			
			}
			//Render a frame
			RENDERER::RenderFrame(registry, rendererEntity);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		RENDERER::CleanupRenderer(registry, rendererEntity);
	}
	catch (const std::exception& error)
	{
		std::cerr << "Render thread error: " << error.what() << "\n";
		RENDERER::CleanupRenderer(registry, rendererEntity);
	}

}