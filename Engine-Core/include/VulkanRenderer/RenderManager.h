#pragma once


#include <condition_variable>


class RenderManager
{
public:

	RenderManager();
	~RenderManager();

	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager) = delete;

	void startup();
	void shutdown();

private:


	std::thread renderThread;
	std::mutex renderLock;
	std::condition_variable cv;
	
};