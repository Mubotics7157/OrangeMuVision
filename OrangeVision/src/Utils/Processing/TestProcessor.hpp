#pragma once
#ifndef TEST_PROCESSOR_HPP
#define TEST_PROCESSOR_HPP
#include "ImageProcessor.hpp"

class TestProcessor : public ImageProcessor {
public:
	TestProcessor(std::shared_ptr<ConcurrentMat> inputStream, std::shared_ptr<ConcurrentJson> outputStream) : ImageProcessor(inputStream, outputStream) {
	}

	nlohmann::json process(cv::Mat& img) {
		nlohmann::json json;
		json["time"] = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
		cv::imshow("test", img);
		cv::waitKey(1);
		return json;
	}
};

#endif