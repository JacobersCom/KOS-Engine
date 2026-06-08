#Cmake make minium required 
cmake_minimum_required(VERSION 4.0)

#Project name version and language used
project(KOS-Engine VERSION 1.0 LANGUAGES CXX)

#Set the standard language version
set(CMAKE_CXX_STANDARD 17)
#Make it required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")

set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)

add_subdirectory(${VENDOR_DIR}/SDL EXCLUDE_FROM_ALL)

#Adding both subdirectorys
add_subdirectory(App)
add_subdirectory(Engine-Core)
