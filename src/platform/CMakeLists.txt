#Add a subdirectory to platform
add_subdirectory(win32)

#forwards win32 usage requierments to where ever it links
add_library(platform INTERFACE)
target_link_libraries(platform INTERFACE win32)