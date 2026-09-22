
find_package(Vulkan REQUIRED)

set(CORE_PATH           "${CMAKE_CURRENT_SOURCE_DIR}/Core")
set(PLATFORM_PATH    "${CMAKE_CURRENT_SOURCE_DIR}/Platform")
set(RHI_PATH             "${CMAKE_CURRENT_SOURCE_DIR}/RHI")

set(WINDOWS_PATH "${PLATFORM_PATH}/Windows")
set(VULKAN_PATH        "${RHI_PATH}/Vulkan")

file(GLOB_RECURSE SRC_WIN
    "${WINDOWS_PATH}/*.cpp"
    "${WINDOWS_PATH}/*.h"
)

file(GLOB_RECURSE SRC_VK
    "${VULKAN_PATH}/*.cpp"
    "${VULKAN_PATH}/*.h"
)

add_library(Platform ${SRC_WIN})
add_library(RHI ${SRC_VK})

source_group("Windows" FILES ${SRC_WIN})
source_group("Vulkan" FILES ${SRC_VK})

set(PRIVATE_SOURCES
    ${VULKAN_PATH}/KRender.cpp
    ${WINDOWS_PATH}/KWindow.cpp
)

set(PUBLIC_SOURCES
    ${VULKAN_PATH}/KRender.h
    ${WINDOWS_PATH}/KWindow.h
)


# Link Vulkan include/library to the engine target
if (Vulkan_FOUND)
    # Prefer imported target when available
    if (TARGET Vulkan::Vulkan)
        target_link_libraries(RHI PRIVATE Vulkan::Vulkan)
    else()
        target_include_directories(KOS-Engine PRIVATE ${Vulkan_INCLUDE_DIR})
        target_link_libraries(KOS-Engine PRIVATE ${Vulkan_LIBRARY})
    endif()
else()
    message(FATAL_ERROR "Vulkan SDK not found. Please install the Vulkan SDK and set VULKAN_SDK environment variable.")
endif()

#Set source and output folders
#set(SHADER_SRC_DIR "${PROJECT_SOURCE_DIR}/Shaders")
#set(HLSL_VERT "${SHADER_SRC_DIR}/VertexShader.hlsl")
#set(HLSL_FRAG "${SHADER_SRC_DIR}/PixelShader.hlsl")


#target_sources(KOS-Engine PRIVATE
#    "${HLSL_VERT}"
#    "${HLSL_FRAG}"
#
#)



#source_group("Shaders" FILES
#    "${HLSL_VERT}"
#    "${HLSL_FRAG}"
#)





