find_package(Vulkan QUIET)


add_executable(KOS main.cpp)

target_sources(KOS
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/KRender.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KMat4.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KVector3.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KRender.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KWindow.hpp

)

target_link_libraries(KOS
PRIVATE
    Vulkan::Vulkan
)

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


source_group("Shaders" FILES
    "${HLSL_VERT}"
    "${HLSL_FRAG}"
    "${SPV_VERT}"
    "${SPV_FRAG}"
)

target_sources(KOS PRIVATE
    "${HLSL_VERT}"
    "${HLSL_FRAG}"
    "${SPV_VERT}"
    "${SPV_FRAG}"
)


set_source_files_properties(
    "${HLSL_VERT}"
    "${HLSL_FRAG}"
    PROPERTIES HEADER_FILE_ONLY ON
)

target_compile_definitions(KOS
    PRIVATE KENGINE_SHADER_DIR="${SHADER_OUT_DIR}"
)
