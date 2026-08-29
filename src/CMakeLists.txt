find_package(Vulkan REQUIRED)

target_sources(KOS-Engine
PRIVATE
	KWindow.cpp
	KRender.cpp
	KLog.cpp
	KVector3.cpp
	
PUBLIC
	KWindow.h
	KRender.h
	KLog.h
	KVector3.h
	KMat4.h
)

# Link Vulkan include/library to the engine target
if (Vulkan_FOUND)
    # Prefer imported target when available
    if (TARGET Vulkan::Vulkan)
        target_link_libraries(KOS-Engine PRIVATE Vulkan::Vulkan)
    else()
        target_include_directories(KOS-Engine PRIVATE ${Vulkan_INCLUDE_DIR})
        target_link_libraries(KOS-Engine PRIVATE ${Vulkan_LIBRARY})
    endif()
else()
    message(FATAL_ERROR "Vulkan SDK not found. Please install the Vulkan SDK and set VULKAN_SDK environment variable.")
endif()

#Find the shader compiler 
find_program(GLSLC
	name glslc
	HINTS ENV VULKAN_SDK
	PATH_SUFFIXES Bin bin
	REQUIRED
)

#Set source and output folders
set(SHADER_SRC_DIR "${PROJECT_SOURCE_DIR}/Shaders")
set(SHADER_OUT_DIR "${CMAKE_BINARY_DIR}/shaders")

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
    PROPERTIES
)



target_compile_definitions(KOS-Engine
    PRIVATE KENGINE_SHADER_DIR="${SHADER_OUT_DIR}"
)
