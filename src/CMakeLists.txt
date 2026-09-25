
# Sets up paths to folders
set(CORE_PATH           "${CMAKE_CURRENT_SOURCE_DIR}/Core")
set(RHI_PATH            "${CMAKE_CURRENT_SOURCE_DIR}/RHI")
set(UTILS_PATH          "${CMAKE_CURRENT_SOURCE_DIR}/Utils")

#RHI module 
find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

file(GLOB_RECURSE Vulkan
    "${RHI_PATH}/Vulkan/*.cpp"
    "${RHI_PATH}/Vulkan/*.h"
)

file(GLOB_RECURSE SRC_RHI
    ${Vulkan}
)

add_library(RHI ${SRC_RHI})

source_group("Vulkan" FILES ${Vulkan})

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

file(GLOB_RECURSE SRC_CORE
    ${Logger}
    ${Math}
    ${Window}
)
add_library(Core ${SRC_CORE})

source_group("Math" FILES ${Math})
source_group("Logger" FILES ${Logger})
source_group("Window" FILES ${Window})

#end of Core module

#Utils module
file(GLOB_RECURSE SRC_UTILS
    "${UTILS_PATH}/*.h"
    "${UTILS_PATH}/*.cpp"
)
add_library(Utils ${SRC_UTILS})

source_group("Utils" FILES ${SRC_UTILS})
set_target_properties(Utils PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Utils PUBLIC "../third_party/")

#end of Utils module

target_include_directories(Core PUBLIC "../third_party/")

target_link_libraries(Core PRIVATE Utils)


target_link_libraries(KOS-Engine PUBLIC Core)
target_compile_features(KOS-Engine PUBLIC cxx_std_17)

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





