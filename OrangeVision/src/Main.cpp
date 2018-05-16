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


int main() {
	//Create a camera
	std::shared_ptr<cv::VideoCapture> capture = std::make_shared<cv::VideoCapture>(0);
	//Create a reader to read camera
	std::shared_ptr<ImageReader> reader = std::make_shared<ImageReader>(capture);
	//Create a processor that that takes in the reader's output
	std::shared_ptr<TestProcessor> processor = std::make_shared<TestProcessor>(reader->getImgStream());
	//Create a handler to handle the output from the processor
	std::shared_ptr<FakeHandler> fakeHandler = std::make_shared<FakeHandler>(processor->getJsonOutput());
	OrangeThread readingThread({reader, fakeHandler});
	OrangeThread processingThread({processor}); 
	while (true) {

	}
};