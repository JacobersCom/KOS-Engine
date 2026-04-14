find_package(Vulkan QUIET)


add_executable(KOS-Engine main.cpp)


set(KECS_P "${PROJECT_SOURCE_DIR}/src/KECS/")

target_sources(KOS-Engine
PRIVATE
	
	${KECS}/KECS.cpp
)

add_subdirectory(KOS-Engine-Core)
add_subdirectory(RenderingSystems)

target_link_libraries(KOS-Engine 
PUBLIC 
    KOS-Engine-Core
    RenderingSystems
)
