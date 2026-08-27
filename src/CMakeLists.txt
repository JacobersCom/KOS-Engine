find_package(Vulkan QUIET)

include(FetchContent)
FetchContent_Declare(
    fetch_vk_bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG main #suggest using a tag so the library doesn't update whenever new commits are pushed to a branch
)
FetchContent_MakeAvailable(fetch_vk_bootstrap)

add_executable(KOS-Engine main.cpp)
target_link_libraries(KOS-Engine PRIVATE 
    Vulkan::Vulkan
    vk-bootstrap::vk-bootstrap
)

target_sources(KOS-Engine
PRIVATE
	KWindow.cpp
	KRender.cpp
	
PUBLIC
	KWindow.h
	KRender.h
)

#Find the shader compiler 
find_program(GLSLC
	name glslc
	HINTS ENV VULKAN_SDK
	PATH_SUFFIXES Bin bin
	REQUIRED
)

#Set source and output folders
set(SHADER_SRC_DIR "${PROJECT_SOURCE_DIR}/Shaders")
set(SHADER_OUT_DIR "${CMAKE_BINARY_DIR}//shaders")

#Set HLSL source files
set(HLSL_VERT "${SHADER_SRC_DIR}/VertexShader.hlsl")
set(HLSL_FRAG "${SHADER_SRC_DIR}/PixelShader.hlsl")

set(SPV_VERT "${SHADER_OUT_DIR}/VertShader.spv")
set(SPV_FRAG "${SHADER_OUT_DIR}/FragShader.spv")

add_custom_command(
	OUTPUT "${SPV_VERT}"
	COMMAND  ${CMAKE_COMMAND} -E make_directory "${SHADER_OUT_DIR}"
    COMMAND "${GLSLC}"
            -fshader-stage=vert
            -fentry-point=main
            --target-env=vulkan1.4
            -o "${SPV_VERT}"
            "${HLSL_VERT}"
    DEPENDS "${HLSL_VERT}"
    VERBATIM
)

# Fragment shader
add_custom_command(
    OUTPUT "${SPV_FRAG}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_OUT_DIR}"
    COMMAND "${GLSLC}"
            -fshader-stage=frag
            -fentry-point=main
            --target-env=vulkan1.4
            -o "${SPV_FRAG}"
            "${HLSL_FRAG}"
    DEPENDS "${HLSL_FRAG}"
    VERBATIM
)

target_sources(KOS-Engine PRIVATE
    "${HLSL_VERT}"
    "${HLSL_FRAG}"
    "${SPV_VERT}"
    "${SPV_FRAG}"
)

source_group("Shaders" FILES
    "${HLSL_VERT}"
    "${HLSL_FRAG}"
)

source_group("Generated Shaders" FILES
    "${SPV_VERT}"
    "${SPV_FRAG}"
)

set_source_files_properties(
    "${HLSL_VERT}"
    "${HLSL_FRAG}"
    PROPERTIES HEADER_FILE_ONLY ON
)

target_compile_definitions(KOS-Engine
    PRIVATE KENGINE_SHADER_DIR="${SHADER_OUT_DIR}"
)
