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
#include "Utils\OrangeThread.hpp"

nlohmann::json testFunc(cv::Mat& img) {
	nlohmann::json json;
	cv::imshow("test", img);
	cv::waitKey(1);
	return json;
}

nlohmann::json testFunc2(cv::Mat& img) {
	cv::Mat threshold;
	cv::cvtColor(img, threshold, cv::COLOR_BGR2GRAY);
	nlohmann::json json;
	cv::imshow("test", threshold);
	cv::waitKey(1);
	return json;
}

int main() {
	std::shared_ptr<CameraUtils::ConcurrentMat> stream = std::make_shared<CameraUtils::ConcurrentMat>();
	std::shared_ptr<ImageReader> reader = std::make_shared<ImageReader>(stream);
	reader->open(0); // Writes VideoCapture(0).read() into ConcurrentMat
	std::shared_ptr<ImageProcessor> processor = std::make_shared<ImageProcessor>(stream); // Reads from ConcurrentMat
 	processor->setProcessingFunction(&testFunc); // Set whatever function you want the processor to call.
	// It needs to take in a cv::Mat& and return a nlohmann::json. nlohmann::json is an object from a 
	// header only json library we use. 
	OrangeThread<ImageReader> readingThread(reader); // create threads
	OrangeThread<ImageProcessor> processingThread(processor); // they run when they are constructed
	//call readingThread.stop() or processingThread.stop() to pause threads from running. 
	// They automatically terminate when it exits the scope.
	//Wait is here because reader and processor are cleaned up automatically when the scope exits (aka when the destructor is called)
	int i = 0;
	while (true) {
		if (i % 2 == 1) {
			processor->setProcessingFunction(&testFunc);
		} else {
			processor->setProcessingFunction(&testFunc2);
		}
		++i;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
};