cmake_minimum_required(VERSION 2.6)
project(apriltags)


set(CMAKE_MODULE_PATH
    ${CMAKE_BINARY_DIR}
    ${CMAKE_MODULE_PATH}
   )

file(GLOB SOURCE_FILES "src/*.cc")
include_directories(AprilTags . )
add_library(apriltags ${SOURCE_FILES})

include_directories(${CMAKE_INSTALL_PREFIX}/include/eigen3)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${EIGEN3_INCLUDE_DIR})

target_link_libraries(apriltags ${OpenCV_LIBS}) #-pg) #-fopenmp)

#add_subdirectory(example)

# Add stuff for an install target.
file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/AprilTags/*)
install(PROGRAMS ${HEADER_FILES} DESTINATION include/AprilTags COMPONENT applications)
install(TARGETS apriltags
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
  
  )
