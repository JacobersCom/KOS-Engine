find_package(Vulkan QUIET)

add_library(Engine-Core STATIC)

target_sources(Engine-Core
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanBuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.cpp

PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanBuffers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanComponents.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanSystems.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/RenderManager.h
)

target_include_directories(Engine-Core

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

include(FetchContent)
FetchContent_Declare(
    fetch_vk_bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG        BRANCH_OR_TAG #suggest using a tag so the library doesn't update whenever new commits are pushed to a branch
)
FetchContent_MakeAvailable(fetch_vk_bootstrap)

target_link_libraries(Engine-Core 
PRIVATE 
    Vulkan::Vulkan
    vk-bootstrap::vk-bootstrap
)

target_compile_definitions(Engine-Core PUBLIC cxx_std_17)

