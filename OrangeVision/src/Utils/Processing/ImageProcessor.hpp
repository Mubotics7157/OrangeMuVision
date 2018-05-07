#pragma once
#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP
#include <condition_variable>
#include <atomic>
#include <thread>
#include <functional>
#include "Utils\Threading\ConcurrentMat.hpp"
#include "nholmann\json.hpp"
#include "Utils\Threading\Updateable.hpp"
#include "Utils\Output\OutputHandler.hpp"
#include "Utils\Threading\ConcurrentJson.hpp"

class ImageProcessor : public Updateable {
public:
	ImageProcessor(std::shared_ptr<ConcurrentMat> inputStream, std::shared_ptr<ConcurrentJson> outputStream);
	void setInputStream(std::shared_ptr<ConcurrentMat> inputStream);
	void setOutputStream(std::shared_ptr<ConcurrentJson> outputStream);
	void update();
	virtual nlohmann::json process(cv::Mat& img) =0;
private:
	cv::Mat m_img;
	unsigned int m_lastId = 0;
	std::shared_ptr<ConcurrentMat> m_inputStream;
	std::shared_ptr<ConcurrentJson> m_outputStream;
	std::mutex m_inputLock;
	std::mutex m_outputLock;
};

#endif