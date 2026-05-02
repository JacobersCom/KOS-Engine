add_executable(App main.cpp)

target_sources(App
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/App.cpp
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/App.h
)

target_include_directories(App

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/

	PRIVATE
    ${PROJECT_SOURCE_DIR}/deps/
)

target_link_libraries(App 
PUBLIC
	Engine-Core
)

target_compile_features(App PRIVATE cxx_std_17)