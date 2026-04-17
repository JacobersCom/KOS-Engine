
add_executable(Engine-Test main_test.cpp)

#Link Engine test to the engine core
# target_link_libraries(Engine-Test 
# PRIAVTE
    # Engine-Core
# )

target_include_directories(Engine-Test 
PRIVATE
	${PROJECT_SOURCE_DIR}/Engine-Core/include/ecs/
)

target_compile_features(Engine-Test PRIVATE cxx_std_17)

#Enables cmake testing
enable_testing()

#Test for the engine core
add_test(NAME EngineCoreTest COMMAND EngineTest)