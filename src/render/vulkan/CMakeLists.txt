
#Create a library called vulkan that can be used to link back to main
add_library(vulkan)

#All the src files held with the vulkan lib. Being these are src files they are private
target_sources(vulkan
PRIVATE
    KRender.cpp
)

#Links the src files to the renderer header files
target_include_directories(vulkan
PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

#Links the Vulkan SDK to the KRender header file
target_link_libraries(vulkan
PUBLIC
    Vulkan::Vulkan
)
