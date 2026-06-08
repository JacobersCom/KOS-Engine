


find_package(Vulkan QUIET)

add_library(KOS STATIC)
add_library(KOS::KOS ALIAS KOS)

target_sources(KOS
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.cpp
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/KOSWindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderManager.hpp
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.h
)

source_group("deps" FILES
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.h
    ${PROJECT_SOURCE_DIR}/deps/VkBootstrap.cpp
    
)



target_include_directories(KOS

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/

    ${PROJECT_SOURCE_DIR}/deps/
)

target_link_libraries(KOS 
PUBLIC 
    Vulkan::Vulkan
    SDL3::SDL3
)

target_compile_features(KOS PUBLIC cxx_std_17)

