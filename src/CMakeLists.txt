find_package(Vulkan QUIET)

add_subdirectory(Platform)
add_subdirectory(RHI)


add_executable(KOS-Engine main.cpp)

target_include_directories(KOS-Engine 
PUBLIC 
    RHI
    Platform
)
