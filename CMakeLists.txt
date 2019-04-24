cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(OrangeVision)

find_package(OpenCV REQUIRED)


add_subdirectory(OV)

include_directories(${CMAKE_INSTALL_PREFIX}/include/eigen3)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/)
include_directories(${CMAKE_SOURCE_DIR}/OV/include/)
include_directories(${CMAKE_SOURCE_DIR}/OV/)
add_executable(OrangeVision main.cpp)

target_link_libraries(OrangeVision OV)


