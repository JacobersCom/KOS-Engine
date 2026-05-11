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

	auto& sync = registry.ctx().emplace<RENDERER::RenderSync>();


	auto surfaceView = registry.view<RENDERER::Surface>();
	
	//Set render thread surface flag
	sync.state->surfaceReady = surfaceView.begin() != surfaceView.end();

	/* Creates a new thread that creates the render manager
	and ensures registry is a & */
	renderThread = std::thread(&RenderManager::StartRenderThread, this,std::ref(registry));
}

RenderManager::~RenderManager()
{
	RequestShutDown();

	if (renderThread.joinable())
	{
		renderThread.join();
	}
}

void RenderManager::RequestShutDown()
{
	if (registryRef == nullptr)
	{
		return;
	}

	//Tell all threads to shutdown and stop working
	auto sync = registryRef->ctx().get<RENDERER::RenderSync>();
	{
		std::lock_guard<std::mutex> lock(sync.state->lock);
		sync.state->shutdown = true;
	}
	sync.state->cv.notify_all();
}


void RenderManager::StartRenderThread(entt::registry& registry)
{

	//else get the component from the first entity
	auto& sync = registry.ctx().get<RENDERER::RenderSync>();

	{
		//Locks thread
		std::unique_lock<std::mutex> lock(sync.state->lock);
		
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
}