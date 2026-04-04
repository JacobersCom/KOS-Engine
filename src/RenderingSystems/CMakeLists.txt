
#Create a library called vulkan that can be used to link back to main
add_library(RenderingSystems)

#All the src files held with the vulkan lib. Being these are src files they are private
target_sources(RenderingSystems
PRIVATE
    KRender.cpp
	
PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
        ${PROJECT_SOURCE_DIR}/include/vulkan/KRender.h
)

#Links the src files to the renderer header files
target_include_directories(RenderingSystems
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/vulkan/
	${PROJECT_SOURCE_DIR}/include/win32/
)

#Links the Vulkan SDK to the KRender header file
target_link_libraries(RenderingSystems
PUBLIC
    Vulkan::Vulkan
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
set(SHADER_OUT_DIR "${CMAKE_BINARY_DIR}/Shaders")

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

target_sources(RenderingSystems PRIVATE
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

target_compile_definitions(RenderingSystems
    PRIVATE KENGINE_SHADER_DIR="${SHADER_OUT_DIR}"
)