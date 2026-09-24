
# Sets up paths to folders
set(CORE_PATH           "${CMAKE_CURRENT_SOURCE_DIR}/Core")
set(PLATFORM_PATH    "${CMAKE_CURRENT_SOURCE_DIR}/Platform")
set(RHI_PATH             "${CMAKE_CURRENT_SOURCE_DIR}/RHI")

set(WINDOWS_PATH "${PLATFORM_PATH}/Windows")
set(VULKAN_PATH        "${RHI_PATH}/Vulkan")

#Sources for windows platform and VK
#Note: Will need to change the GLOB_RECURSE to target the platform and RHI folder instead
file(GLOB_RECURSE SRC_WIN
    "${WINDOWS_PATH}/*.cpp"
    "${WINDOWS_PATH}/*.h"
)

file(GLOB_RECURSE SRC_VK
    "${VULKAN_PATH}/*.cpp"
    "${VULKAN_PATH}/*.h"
)

find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

add_library(Platform ${SRC_WIN})
add_library(RHI ${SRC_VK})

target_link_libraries(RHI PRIVATE Vulkan::Vulkan Vulkan::shaderc_combined)

source_group("Windows" FILES ${SRC_WIN})
source_group("Vulkan" FILES ${SRC_VK})



target_link_libraries(KOS-Engine PRIVATE RHI Platform)

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





