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


int main() {
	std::shared_ptr<ConcurrentMat> imgStream = std::make_shared<ConcurrentMat>();
	std::shared_ptr<ConcurrentJson> jsonStream = std::make_shared<ConcurrentJson>();
	std::shared_ptr<cv::VideoCapture> capture = std::make_shared<cv::VideoCapture>("C:/Users/Roth Vann/Documents/hey.avi");
	std::shared_ptr<FakeHandler> fakeHandler = std::make_shared<FakeHandler>(jsonStream);
	std::shared_ptr<ImageReader> reader = std::make_shared<ImageReader>(imgStream, capture);
	std::shared_ptr<TestProcessor> processor = std::make_shared<TestProcessor>(imgStream, jsonStream); // Reads from ConcurrentMat
	// It needs to take in a cv::Mat& and return a nlohmann::json. nlohmann::json is an object from a 
	// header only json library we use. 
	OrangeThread readingThread({reader, fakeHandler}); // create threads
	OrangeThread processingThread({processor}); // they run when they are constructed
	//call readingThread.stop() to pause threads from running. They automatically terminate when it exits the scope.
	//Wait is here because reader and processor are cleaned up automatically when the scope exits (aka when the destructor is called)
	while (true) {

	}
};