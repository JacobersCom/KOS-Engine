find_package(Vulkan QUIET)

add_library(KOS-Engine-Main)
add_subdirectory(win32)
add_subdirectory(RenderingSystems)

add_executable(KOS-Engine main.cpp)

target_link_libraries(KOS-Engine 
PUBLIC 
    RenderingSystems
)
