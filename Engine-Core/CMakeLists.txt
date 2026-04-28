find_package(Vulkan QUIET)

include(FetchContent)
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.13.1  # Specify a specific version or commit hash
)

add_library(Engine-Core STATIC)

target_sources(Engine-Core
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanBuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.cpp
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.cpp


PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanBuffers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanComponents.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanSystems.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/KOSVulkanWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VulkanRenderer/RenderManager.h
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.h

)

target_include_directories(Engine-Core

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(Engine-Core 
PRIVATE 
    Vulkan::Vulkan
    EnTT::EnTT
)

target_compile_definitions(Engine-Core PUBLIC cxx_std_17)

