find_package(Vulkan QUIET)


add_executable(KOS-Engine main.cpp)

add_subdirectory(KOS-Engine-Core)
add_subdirectory(RenderingSystems)

target_link_libraries(KOS-Engine 
PUBLIC 
    RenderingSystems
)
