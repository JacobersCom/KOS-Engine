add_subdirectory(vulkan)

add_library(render)

target_link_libraries(render PRIVATE vulkan)