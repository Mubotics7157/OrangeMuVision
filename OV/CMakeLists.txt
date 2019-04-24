project(OV)

#add_subdirectories(Input)
set(INPUT_SOURCES 
	Input/ImageReader.cpp
	Input/Undistort.cpp
)
set(INPUT_INCLUDES
	Input/ImageReader.hpp
	Input/Undistort.hpp
)
#add_subdirectory(Output)
set(OUTPUT_SOURCES)
set(OUTPUT_INCLUDES
	Output/Display.hpp
	Output/FakeHandler.hpp
)
#add_subdirectory(Processing)
set(PROCESSING_SOURCES
	Processing/AprilTagDetector.cpp
)
set(PROCESSING_INCLUDES
	Processing/AprilTagDetector.hpp
	Processing/DrawClasses.hpp
	Processing/Localization.hpp
	Processing/TestProcessor.hpp
)
#add_subdirectory(Threading)
set(THREADING_SOURCES
	Threading/OrangeThread.cpp
)
set(THREADING_INCLUDES
	Threading/ConcurrentStream.hpp
	Threading/ConcurrentStreamReader.hpp
	Threading/OrangeThread.hpp
	Threading/Updateable.hpp
)
#add_subdirectory(Utils)
set(UTILS_SOURCES 
	Utils/Camera/CalibrationUtil.cpp
	Utils/Camera/Calibrator.cpp
	Utils/Network/UDPListener.cpp
	Utils/Network/UDPSender.cpp
)
set(UTILS_INCLUDES
	Utils/Camera/CalibrationUtil.hpp
	Utils/Camera/Calibrator.hpp
	Utils/Network/UDPListener.hpp
	Utils/Network/UDPSender.hpp
	Utils/IdToClass.hpp
	Utils/TextToPoints.hpp
)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_INSTALL_PREFIX}/include/AprilTags/)
include_directories(${CMAKE_INSTALL_PREFIX}/include/eigen3/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/)
include_directories(include .)

add_library(OV 
	${INPUT_SOURCES}
	${INPUT_INCLUDES}
	${OUTPUT_SOURCES}
	${OUTPUT_INCLUDES}
	${PROCESSING_SOURCES}
	${PROCESSING_INCLUDES}
	${THREADING_SOURCES}
	${THREADING_INCLUDES}
	${UTILS_SOURCES}
	${UTILS_INCLUDES}
)

target_link_libraries(OV ${OpenCV_LIBS})
target_link_libraries(OV ${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a)
target_link_libraries(OV Threads::Threads)
