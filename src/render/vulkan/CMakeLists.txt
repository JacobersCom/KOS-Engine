
#Create a library called vulkan that can be used to link back to main
add_library(vulkan)

#All the src files held with the vulkan lib. Being these are src files they are private
target_sources(vulkan
PRIVATE
    )

#Links the 
target_include_directories(vulkan
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/render
)

