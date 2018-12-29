//#include "Processing\AprilTagDetector.hpp"
#include "opencv2\opencv.hpp"
//#include "AprilTags\include\Tag36h15.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include "nholmann\json.hpp"
#include "Utils\Processing\TestProcessor.hpp"
#include "Utils\Input\ImageReader.hpp"
#include "Utils\Threading\OrangeThread.hpp"
#include "Utils\Output\FakeHandler.hpp"
#include "Utils\Threading\ConcurrentStream.hpp"
#include "Utils\Processing\CalibrationUtil.hpp"
#include "Utils\Input\Undistort.hpp"


void calibrate() {
	cv::VideoCapture capture(0);

}

int main() {
	std::string base = std::string("C:/Users/Roth Vann/Documents/Nexus Calib/");
	cv::Mat intrins, distCoeff;
	Calibrator::loadFrom(base + "config.txt", intrins, distCoeff);
	
	std::shared_ptr<cv::VideoCapture> capture = std::make_shared<cv::VideoCapture>(base + "vid5" + ".mp4");
	std::shared_ptr<ImageReader> reader = std::make_shared<ImageReader>(capture);
	std::shared_ptr<Undistort> undistort = std::make_shared<Undistort>(intrins, distCoeff, reader->getImgStream());
	std::shared_ptr<TestProcessor> processor = std::make_shared<TestProcessor>(undistort->getImgStream(), intrins, distCoeff);
	std::shared_ptr<CalibrationUtil> calib = std::make_shared<CalibrationUtil>(0, cv::Size(9, 6), 27.5, reader->getImgStream());
	
	//Create a handler to handle the output from the processor
	//std::shared_ptr<FakeHandler> fakeHandler = std::make_shared<FakeHandler>(processor->getJsonStream());
	
	OrangeThread readingThread({reader});
	OrangeThread processingThread({undistort, processor});
	
	
	/*
	cv::Mat img, img2;
	Calibrator calib(cv::Size(9, 6), 27.5);
	cv::VideoCapture cap(base + "vid2" + ".mp4");
	cv::VideoCapture a("a");
	int iter = 0;
	while (cap.read(img)){
		cv::resize(img, img2, cv::Size(), 0.5, 0.5);
		calib.detect(img2, true);
		++iter;
		if (iter > 150) {
			break;
		}
		//cv::undistort(img2, img, intrins, distCoeff);
		cv::imshow("img", img2);
	}
	calib.calibrate();
	calib.saveTo(base + "config.txt");
	std::cout << "done" << std::endl;
	*/
	while (true) {

	}
}
