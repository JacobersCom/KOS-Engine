add_executable(App main.cpp)



target_link_libraries(App 
PRIVATE
	KOS::KOS
)

target_compile_features(App PRIVATE cxx_std_17)