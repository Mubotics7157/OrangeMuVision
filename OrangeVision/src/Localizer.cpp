//#include "Processing\AprilTagDetector.hpp"
#include "opencv2\opencv.hpp"
//#include "AprilTags\include\Tag36h15.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include "nholmann\json.hpp"
#include "Processing\ImageProcessor.hpp"
#include "Utils\ImageReader.hpp"

nlohmann::json testFunc(cv::Mat& img) {
	nlohmann::json json;
	json["id"] = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	cv::imshow("test", img);
	cv::waitKey(1);
	std::cout << "working" << std::endl;
	return json;
}

int main() {
	std::shared_ptr<CameraUtils::ConcurrentMat> stream = std::make_shared<CameraUtils::ConcurrentMat>();
	ImageReader reader(stream);
	reader.open(0);
	ImageProcessor processor(stream);
	processor.setProcessingFunction(&testFunc);
	reader.start();
	processor.start();

	//Wait is here because reader and processor are cleaned up automatically when the scope exits (aka when the destructor is called)
	while (true) {

	}
};