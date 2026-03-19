add_subdirectory(platform)
add_subdirectory(render)

add_executable(KOS-Engine main.cpp)

target_link_libraries(KOS-Engine PRIVATE platform)
target_link_libraries(KOS-Engine PRIVATE render)