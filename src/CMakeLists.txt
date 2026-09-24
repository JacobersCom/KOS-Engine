
# Sets up paths to folders
set(CORE_PATH           "${CMAKE_CURRENT_SOURCE_DIR}/Core")
set(PLATFORM_PATH    "${CMAKE_CURRENT_SOURCE_DIR}/Platform")
set(RHI_PATH             "${CMAKE_CURRENT_SOURCE_DIR}/RHI")

#RHI module 
find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

file(GLOB_RECURSE Vulkan
    "${RHI_PATH}/Vulkan/*.cpp"
    "${RHI_PATH}/Vulkan/*.h"
)

file(GLOB_RECURSE Utils
    "${RHI_PATH}/Utils/*.cpp"
    "${RHI_PATH}/Utils/*.h"
)

file(GLOB_RECURSE SRC_RHI
    ${Vulkan}
    ${Utils}
)

add_library(RHI ${SRC_RHI})

source_group("Vulkan" FILES ${Vulkan})
source_group("Utils" FILES ${Utils})


target_include_directories(RHI PUBLIC "../third_party/Gateware.h")
target_link_libraries(RHI PRIVATE Vulkan::Vulkan Vulkan::shaderc_combined)
#end of RHI module

#Core module
file(GLOB_RECURSE Logger
    "${CORE_PATH}/Logger/*.cpp"
    "${CORE_PATH}/Logger/*.h"
)

file(GLOB_RECURSE Math
    "${CORE_PATH}/Math/*.cpp"
    "${CORE_PATH}/Math/*.h"    
)

file(GLOB_RECURSE Window
    "${CORE_PATH}/Window/*.cpp"
    "${CORE_PATH}/Window/*.h"    
)

file(GLOB_RECURSE Utils
    "${CORE_PATH}/Utils/*.cpp"
    "${CORE_PATH}/Utils/*.h"    
)

file(GLOB_RECURSE SRC_CORE
    ${Logger}
    ${Math}
    ${Window}
    ${Utils}

)
add_library(Core ${SRC_CORE})

source_group("Math" FILES ${Math})
source_group("Utils" FILES ${Utils})
source_group("Logger" FILES ${Logger})
source_group("Window" FILES ${Window})

#end of Core module

target_link_libraries(KOS-Engine PRIVATE RHI Core)
target_compile_features(KOS-Engine PUBLIC cxx_std_23)

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





