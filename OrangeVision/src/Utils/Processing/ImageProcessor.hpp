#pragma once
#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP
#include <condition_variable>
#include <atomic>
#include <thread>
#include <functional>
#include "Utils\Camera\ConcurrentMat.hpp"
#include "nholmann\json.hpp"
#include "Utils\Threading\Updateable.hpp"

class ImageProcessor : public Updateable {
public:
	ImageProcessor(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream);
	~ImageProcessor();
	void setStream(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream);
	void setProcessingFunction(std::function<nlohmann::json(cv::Mat& img)> processingFunc);
	void getOutput(nlohmann::json& output) const;
	void update();
private:
	cv::Mat img;
	unsigned int lastId = 0;
	std::shared_ptr<CameraUtils::ConcurrentMat> m_imgStream;
	std::mutex m_streamLock;
	std::mutex m_funcLock;
	mutable std::shared_mutex m_jsonLock;
	std::function<nlohmann::json(cv::Mat& img)> m_processingFunc{ nullptr };
	nlohmann::json m_latestOutput;
};

#endif