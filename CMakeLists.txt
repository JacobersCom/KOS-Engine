#Cmake make minium required 
cmake_minimum_required(VERSION 3.20)

#Project name version and language used
project(KOS-Engine VERSION 1.0 LANGUAGES CXX)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

#Set the standard language version
set(CMAKE_CXX_STANDARD 17)
#Make it required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(KOS-Engine src/main.cpp)



#Adding both subdirectorys
add_subdirectory(src)


