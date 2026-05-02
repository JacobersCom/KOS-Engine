add_executable(App main.cpp)

target_link_libraries(App 
PUBLIC
	Engine-Core
)

target_compile_features(App PRIVATE cxx_std_17)