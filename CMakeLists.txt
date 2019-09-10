cmake_minimum_required(VERSION 2.8.9)
project(amplitude)

#Compile flags
set (CMAKE_CXX_FLAGS "-O2 -g")


#Headers
include_directories(include)
include_directories(lib)
 
set (PROJECT_LINK_LIBS libglfw.so libglfw.so.3 libdl.so.2)
link_directories(lib)

#Source files
file(GLOB SOURCES "src/*.c*")

add_executable(amplitude.x ${SOURCES})

target_link_libraries(amplitude.x ${PROJECT_LINK_LIBS})
