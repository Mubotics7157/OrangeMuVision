#include "opencv2\opencv.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include "nholmann\json.hpp"
#include "OrangeVision.hpp"

int main() {	
	cv::Mat intrins, distCoeff;
	ov::Calibrator::loadFrom("C:/Users/rothv/source/repos/OrangeVision/OrangeVision/config.txt", intrins, distCoeff);
	//Create a camera
	std::shared_ptr<cv::VideoCapture> capture = std::make_shared<cv::VideoCapture>(0);
	auto reader = std::make_shared<ov::ImageReader>(capture);

	auto undistort = std::make_shared<ov::Undistort>(intrins, distCoeff, reader->getImgStream());
	auto processor = std::make_shared<ov::TestProcessor>(undistort->getImgStream(), intrins, distCoeff);
	auto calib = std::make_shared <ov::CalibrationUtil > (0, cv::Size(9, 6), 1, reader->getImgStream());

	ov::OrangeThread readingThread({reader});
	ov::OrangeThread processingThread({undistort, processor});
	ov::OrangeThread calibThread({calib});
	calibThread.stop();
	while (true) {
		char key = cv::waitKey(1);
		if (key == 'e') {
			std::cout << "start";
			calibThread.start();
			processingThread.stop();
		}
		if (key == 'q') {
			calibThread.stop();
			ov::Calibrator::loadFrom("config.txt", intrins, distCoeff);
			processor->updateIntrins(intrins);
			processor->updateDistCoeff(distCoeff);
			processingThread.start();
		}
	}

}
