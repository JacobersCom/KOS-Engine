#include "RenderManager.hpp"

#include <exception>
#include <iostream>
#include <mutex>
#include <thread>




RenderManager::RenderManager()
{

	//registry.ctx().emplace<RENDERER::RenderSync>();

	/* Creates a new thread that creates the render manager
	and ensures registry is a & */
	//renderThread = std::thread(&RenderManager::startup);
}

RenderManager::~RenderManager()
{
	shutdown();

	//if (renderThread.joinable())
	//{
	//	renderThread.join();
	//}
}

void RenderManager::shutdown()
{
	//if (registryRef == nullptr)
	//{
	//	return;
	//}

	////Tell all threads to shutdown and stop working
	//auto sync = registryRef->ctx().get<RENDERER::RenderSync>();
	//{
	//	std::lock_guard<std::mutex> lock(sync.state->lock);
	//	sync.state->shutdown = true;
	//}
	//sync.state->cv.notify_all();
}


void RenderManager::startup()
{

	////else get the component from the first entity

	//{
	//	//Locks thread
	//	std::unique_lock<std::mutex> lock(renderLock);
	//	
	//	//Unlock thread if either the surface is ready or shutdown was called
	//	cv.wait(lock, [] {

	//		return  shutdown;
	//	});
	//	
	//	if (sync.state->shutdown)
	//	{
	//		return;
	//	}
	//}

	////Create new rendererEntity
	//entt::entity rendererEntity = registry.create();
	//registry.emplace<RENDERER::VulkanData>(rendererEntity);
}