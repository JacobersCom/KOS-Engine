add_library(win32)

target_sources(win32
PRIVATE
    KWindow.cpp
PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
        ${PROJECT_SOURCE_DIR}/include/platform/win32/KWindow.h
)

target_include_directories(win32
PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
