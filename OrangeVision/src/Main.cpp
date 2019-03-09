#include "Processing\AprilTagDetector.hpp"
#include "opencv2\opencv.hpp"
#include "AprilTags\include\Tag36h15.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include "nholmann\json.hpp"
#include "Processing\TestProcessor.hpp"
#include "Input\ImageReader.hpp"
#include "Threading\OrangeThread.hpp"
#include "Output\FakeHandler.hpp"
#include "Threading\ConcurrentStream.hpp"
#include "Utils\CalibrationUtil.hpp"
#include "Input\Undistort.hpp"


void calibrate() {
	cv::VideoCapture capture(0);
}

int main() {	
	cv::Mat intrins, distCoeff;
	Calibrator::loadFrom("C:/Users/rothv/source/repos/OrangeVision/OrangeVision/config.txt", intrins, distCoeff);
	//Create a camera
	std::shared_ptr<cv::VideoCapture> capture = std::make_shared<cv::VideoCapture>(0);
	//Create a reader to read camera
	std::shared_ptr<ImageReader> reader = std::make_shared<ImageReader>(capture);
	//Create a processor that that takes in the reader's output
	std::shared_ptr<Undistort> undistort = std::make_shared<Undistort>(intrins, distCoeff, reader->getImgStream());
	std::shared_ptr<TestProcessor> processor = std::make_shared<TestProcessor>(undistort->getImgStream(), intrins, distCoeff);
	//std::shared_ptr<CalibrationUtil> calib = std::make_shared<CalibrationUtil>(0, cv::Size(9, 6), 1, reader->getImgStream());
	//Create a handler to handle the output from the processor
	//std::shared_ptr<FakeHandler> fakeHandler = std::make_shared<FakeHandler>(processor->getJsonStream());
	OrangeThread readingThread({reader});
	OrangeThread processingThread({undistort, processor});
	while (true) {

	}

}
