#Cmake make minium required 
cmake_minimum_required(VERSION 4.0)

#Project name version and language used
project(KOS VERSION 1.0 LANGUAGES CXX)

#Set the standard language version
set(CMAKE_CXX_STANDARD 17)
#Make it required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Adding both subdirectorys
add_subdirectory(src)

