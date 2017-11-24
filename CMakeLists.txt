cmake_minimum_required(VERSION 3.8)
project(OCLRadixSort)

set(CMAKE_CXX_STANDARD 17)

set(OCL_ROOT $ENV{OCL_ROOT} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-unused -Wno-deprecated-declarations -Wno-ignored-attributes")

include_directories( ${OCL_ROOT}/include )
link_directories   ( ${OCL_ROOT}/lib/x86_64 )

set(SOURCE_FILES test/main.cpp include/OCLRadixSort.hpp)

set(TARGET_NAME OCLRadixSort)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_link_libraries(${TARGET_NAME} OpenCL)