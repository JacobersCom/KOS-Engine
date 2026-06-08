


find_package(Vulkan QUIET)

add_library(Engine-Core STATIC)

target_sources(Engine-Core
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.cpp
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.cpp

PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/RenderManager.h
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.h
)

source_group("deps" FILES
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.h
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.cpp
    
)

target_include_directories(Engine-Core

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/

    PRIVATE
    ${PROJECT_SOURCE_DIR}/deps/
)

target_link_libraries(Engine-Core 
PRIVATE 
    Vulkan::Vulkan
    SDL3::SDL3
)

target_compile_features(Engine-Core PUBLIC cxx_std_17)

