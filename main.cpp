
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include "nholmann/json.hpp"
#include "OrangeVision.hpp"

int main() {	
	cv::Mat intrins, distCoeff;
	ov::Calibrator::loadFrom("./OrangeVision/config.txt", intrins, distCoeff);
	std::shared_ptr<cv::VideoCapture> capture = std::make_shared<cv::VideoCapture>("v4l2src device=/dev/video0 ! video/x-raw, width=640, height=480, framerate=90/1 ! videoconvert ! appsink");

	auto reader = std::make_shared<ov::ImageReader>(capture);
	auto undistort = std::make_shared<ov::Undistort>(intrins, distCoeff, reader->getImgStream());
	auto processor = std::make_shared<ov::FRCVision>(reader->getImgStream());
	auto calib = std::make_shared<ov::CalibrationUtil>(200, cv::Size(9, 6), 1, reader->getImgStream());
  auto stream = std::make_shared<ov::Stream>(processor->getImgStream(), "appsrc ! videoconvert ! video/x-raw, format=I420 ! omxh264enc ! rtph264pay ! udpsink host=192.168.0.155 port=5800");
	ov::OrangeThread readingThread({reader});
	ov::OrangeThread processingThread({processor});
  //ov::OrangeThread streamingThread({stream});
 
	while (true) {
		/*
		char key = cv::waitKey(1);
		if (key == 'e') {
			std::cout << "start";
			processingThread.setUpdateable({calib});
		}
		if (key == 'q') {
			ov::Calibrator::loadFrom("config.txt", intrins, distCoeff);
			processor->updateIntrins(intrins);
			processor->updateDistCoeff(distCoeff);
			processingThread.setUpdateable({undistort, processor});
		}
		*/
	}

}
