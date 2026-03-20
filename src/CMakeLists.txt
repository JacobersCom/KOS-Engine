find_package(Vulkan QUIET)

add_subdirectory(platform)
add_subdirectory(render)

add_executable(KOS-Engine main.cpp)

target_link_libraries(KOS-Engine PUBLIC render)
target_link_libraries(KOS-Engine PUBLIC platform)
