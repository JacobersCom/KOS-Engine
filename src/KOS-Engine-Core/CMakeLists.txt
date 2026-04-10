
set(KOS-CORE-INC "${PROJECT_SOURCE_DIR}/include/KOS-Engine-Core")
set(KOS-CORE-SRC "${PROJECT_SOURCE_DIR}/src/KOS-Engine-Core")
set(WIN-INC "${KOS-CORE-INC}/win32")
set(WIN-SRC "${KOS-CORE-SRC}/win32")


set(SRC 
	${WIN-SRC}/KWindow.cpp
)

set(INC
	${WIN-INC}/KWindow.h
	${KOS-CORE-INC}/KMat4.h
	${KOS-CORE-INC}/KVector3.h
	
)

add_library(KOS-Engine-Core)


target_sources(KOS-Engine-Core
PRIVATE
    ${SRC}
PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/
    FILES
        ${INC}


)

target_include_directories(KOS-Engine-Core
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/

)