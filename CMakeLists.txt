cmake_minimum_required(VERSION 3.8)
project(OCLRadixSort)

set(CMAKE_CXX_STANDARD 17)

set(OCL_ROOT $ENV{OCL_ROOT} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-deprecated-declarations -Wno-ignored-attributes")

include_directories( ${OCL_ROOT}/include )
link_directories   ( ${OCL_ROOT}/lib/x86_64 )

set(SOURCE_FILES include/OCLRadixSort.hpp example/Common.hpp)

set(TARGET_NAME OCLRadixSort)
# trivial example
add_executable(${TARGET_NAME}-trivial ${SOURCE_FILES} example/Trivial.cpp)
target_link_libraries(${TARGET_NAME}-trivial OpenCL)
# speed compare
add_executable(${TARGET_NAME}-speed-compare ${SOURCE_FILES} example/SpeedCompare.cpp)
target_link_libraries(${TARGET_NAME}-speed-compare OpenCL)


