
#include <entt/entt.hpp>
#include "Core/Window/KWindow.h"
#include "Core/Utils/Common.h"


void WindowSystem(entt::registry& registry);

int main()
{
	entt::registry registry;
	
	WindowSystem(registry);

	//Use on construct and on update with entt!!!
}

void WindowSystem(entt::registry& registry)
{
	entt::entity display = registry.create();

	const char* window_name = "KOS-Engine";
	U32 width =		800;
	U32 height =	600;

	registry.emplace<KWindow>(display, KWindow{ window_name, width, height });
}
