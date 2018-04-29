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
	cv::imshow("test", img);
	cv::waitKey(1);
	return json;
}

int main() {
	std::shared_ptr<CameraUtils::ConcurrentMat> stream = std::make_shared<CameraUtils::ConcurrentMat>();
	ImageReader reader(stream);
	reader.open(0); // Writes VideoCapture(0).read() into ConcurrentMat
	ImageProcessor processor(stream); // Reads from ConcurrentMat
	processor.setProcessingFunction(&testFunc); // Set whatever function you want the processor to call.
	// It needs to take in a cv::Mat& and return a nlohmann::json. nlohmann::json is an object from a 
	// header only json library we use. 
	reader.start(); // start reading thread
	processor.start(); // start processing thread
	//call reader.stop() or processor.stop() to close thread. These threads are guaranteed to close in 200 ms.
	//Wait is here because reader and processor are cleaned up automatically when the scope exits (aka when the destructor is called)
	while (true) {

	}
};