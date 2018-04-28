#pragma once
#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP
#include <condition_variable>
#include <atomic>
#include <thread>
#include <functional>
#include "CameraUtils.hpp"
#include "nholmann\json.hpp"

class ImageProcessor {
public:
	ImageProcessor(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream);
	void setStream(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream);
	void setProcessingFunction(std::function<nlohmann::json(cv::Mat& img)> processingFunc);
	void start();
	void stop();
	void getOutput(nlohmann::json& output) const;
private:
	void process();
	std::shared_ptr<CameraUtils::ConcurrentMat> imgStream;
	std::mutex m_funcLock;
	mutable std::shared_mutex m_jsonLock;
	std::atomic<bool> isRunning = false;
	std::function<nlohmann::json(cv::Mat& img)> m_processingFunc{ nullptr };
	std::unique_ptr<std::thread> m_processingThread{ nullptr };
	nlohmann::json m_latestOutput;
};

#endif