//#include "Processing\AprilTagDetector.hpp"
#include "opencv2\opencv.hpp"
//#include "AprilTags\include\Tag36h15.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include "nholmann\json.hpp"
#include "Utils\Processing\ImageProcessor.hpp"
#include "Utils\Camera\ImageReader.hpp"
#include "Utils\Threading\OrangeThread.hpp"

nlohmann::json testFunc(cv::Mat& img) {
	cv::Mat threshold;
	cv::cvtColor(img, threshold, cv::COLOR_BGR2HSV);
	cv::inRange(threshold, cv::Scalar(45, 100, 50), cv::Scalar(80, 255, 255), threshold);
	nlohmann::json json;
	cv::imshow("test", threshold);
	cv::waitKey(1);
	return json;
}

int main() {
	std::shared_ptr<CameraUtils::ConcurrentMat> stream = std::make_shared<CameraUtils::ConcurrentMat>();
	std::shared_ptr<cv::VideoCapture> capture = std::make_shared<cv::VideoCapture>(0);
	std::shared_ptr<ImageReader> reader = std::make_shared<ImageReader>(stream, capture);
	std::shared_ptr<ImageProcessor> processor = std::make_shared<ImageProcessor>(stream); // Reads from ConcurrentMat
	processor->setProcessingFunction(&testFunc); // Set whatever function you want the processor to call.	
	// It needs to take in a cv::Mat& and return a nlohmann::json. nlohmann::json is an object from a 
	// header only json library we use. 
	OrangeThread readingThread(reader); // create threads
	OrangeThread processingThread(processor); // they run when they are constructed
	//call readingThread.stop() to pause threads from running. They automatically terminate when it exits the scope.
	//Wait is here because reader and processor are cleaned up automatically when the scope exits (aka when the destructor is called)
	while (true) {

	}
};