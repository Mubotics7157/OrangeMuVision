//#include "Processing\AprilTagDetector.hpp"
#include "opencv2\opencv.hpp"
//#include "AprilTags\include\Tag36h15.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include "nholmann\json.hpp"
#include "Processing\ImageProcessor.hpp"

nlohmann::json testFunc(cv::Mat& img) {
	nlohmann::json json;
	json["id"] = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	cv::imshow("test", img);
	cv::waitKey(1);
	std::cout << "working" << std::endl;
	return json;
}

void ImageReader(std::shared_ptr<CameraUtils::ConcurrentMat> stream, const char* filename) {
	cv::VideoCapture capture(filename);
	while (true) {
		stream->write(capture);
		cv::waitKey(10);
	}
}

int main() {
	std::shared_ptr<CameraUtils::ConcurrentMat> stream = std::make_shared<CameraUtils::ConcurrentMat>();
	ImageProcessor processor(stream);
	processor.setProcessingFunction(&testFunc);
	std::thread reader(ImageReader, stream, "C:/Users/Roth Vann/Documents/hey.avi");
	processor.start();
	reader.join();
};