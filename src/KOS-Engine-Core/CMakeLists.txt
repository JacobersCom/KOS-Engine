add_library(KOS-Engine-Core)


target_sources(KOS-Engine-Core
PRIVATE
    #source files here
    win32/KWindow.cpp
PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
        ${PROJECT_SOURCE_DIR}/include/KOS-Engine-Core/win32/KWindow.h
        ${PROJECT_SOURCE_DIR}/include/KOS-Engine-Core/KMat4.h
       ${PROJECT_SOURCE_DIR}/include/KOS-Engine-Core/KVector3.h


)

target_include_directories(KOS-Engine-Core
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/KOS-Engine-Core/win32/

)