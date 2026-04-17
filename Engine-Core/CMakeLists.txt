find_package(Vulkan QUIET)

add_library(Engine-Core STATIC)

target_sources(Engine-Core
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/KECS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanBuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSVulkanWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.cpp

PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core/KMat4.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core/KVector3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ecs/KECS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/KOSVulkanBuffers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/KOSVulkanRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/KOSVulkanSystems.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/KOSVulkanWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vulkan/RenderManager.h
)

target_include_directories(Engine-Core

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_compile_definitions(Engine-Core PUBLIC cxx_std_17)