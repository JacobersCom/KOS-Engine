#Add a subdirectory to platform
add_subdirectory(win32)

#Create a platform lib
add_library(platform)

#Link the two libs
target_link_libraries(platform PRIVATE win32)