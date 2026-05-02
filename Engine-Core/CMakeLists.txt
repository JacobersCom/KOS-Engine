


find_package(Vulkan QUIET)

add_library(Engine-Core STATIC)

target_sources(Engine-Core
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.cpp
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanComponents.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanSystems.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/RenderManager.h
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
)

target_compile_definitions(Engine-Core PUBLIC cxx_std_17)

