


find_package(Vulkan QUIET)

add_library(KOS STATIC)
add_library(KOS::KOS ALIAS KOS)

set(VKBOOT_PATH "${PROJECT_SOURCE_DIR}/vendor/VkBootStrap")

target_sources(KOS
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.cpp
    ${VKBOOT_PATH}/VkBootstrap.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.hpp
   ${VKBOOT_PATH}/VkBootstrap.h
)

source_group("vendors" FILES
  ${VKBOOT_PATH}/VkBootstrap.h
  ${VKBOOT_PATH}/VkBootstrap.cpp  
)

target_include_directories(KOS

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/

)

target_link_libraries(KOS 
PUBLIC Vulkan::Vulkan
   PUBLIC SDL3::SDL3
)

target_compile_features(KOS PUBLIC cxx_std_17)

