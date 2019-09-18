cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(OrangeVision)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_subdirectory(OV)

set(APRILTAGS_DIR "C:/Program Files (x86)/apriltags" CACHE PATH "Path to apriltags")

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${APRILTAGS_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/)
include_directories(${CMAKE_SOURCE_DIR}/OV/include/)
include_directories(${CMAKE_SOURCE_DIR}/OV/)
add_executable(OrangeVision main.cpp)

target_link_libraries(OrangeVision OV)


