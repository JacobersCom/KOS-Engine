add_executable(App main.cpp)

target_include_directories(App 
PUBLIC
	${PROJECT_SOURCE_DIR}/Engine-Core/include/
)

target_compile_features(App PRIVATE cxx_std_17)