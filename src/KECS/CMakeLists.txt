add_library(KECS)

target_sources(KECS
PRIVATE
    KECS.cpp

    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES
        ${PROJECT_SOURCE_DIR}/include/KECS/KECS.h
)


target_include_directories(KECS
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/

)