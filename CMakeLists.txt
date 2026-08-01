cmake_minimum_required(VERSION 3.29)
project(DSAProj)
set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
)

add_executable(DSAProj ${SOURCES})
target_include_directories(DSAProj PUBLIC ${CMAKE_SOURCE_DIR}/libs)
target_include_directories(DSAProj PUBLIC ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")