add_library(math
    KMat4.h
    KVector3.h
)

target_link_directories(math
PRIVATE    
    ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)