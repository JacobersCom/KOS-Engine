add_subdirectory(vulkan)

add_library(render INTERFACE)
target_link_libraries(render INTERFACE vulkan)