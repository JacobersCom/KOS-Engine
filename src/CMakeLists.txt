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

#Set source and output folders
set(SHADER_SRC_DIR "${PROJECT_SOURCE_DIR}/Shaders")
set(HLSL_VERT "${SHADER_SRC_DIR}/VertexShader.hlsl")
set(HLSL_FRAG "${SHADER_SRC_DIR}/PixelShader.hlsl")


target_sources(KOS-Engine PRIVATE
    "${HLSL_VERT}"
    "${HLSL_FRAG}"

)



source_group("Shaders" FILES
    "${HLSL_VERT}"
    "${HLSL_FRAG}"
)





