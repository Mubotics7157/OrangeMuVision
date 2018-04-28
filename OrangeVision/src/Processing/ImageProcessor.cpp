#include "ImageProcessor.hpp"

ImageProcessor::ImageProcessor(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream) : imgStream(imgStream) {

}

void ImageProcessor::setStream(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream) {
	this->imgStream = imgStream;
}

void ImageProcessor::setProcessingFunction(std::function<nlohmann::json(cv::Mat& img)> processingFunc) {
	std::unique_lock<std::mutex> lock(m_funcLock);
	m_processingFunc = processingFunc;
}

void ImageProcessor::start() {
	if (m_processingThread) {
		if (!m_processingThread->joinable()) {
			isRunning.store(true, std::memory_order_release);
			m_processingThread = std::make_unique<std::thread>(&ImageProcessor::process, this);
		}
	} else {
		isRunning.store(true, std::memory_order_release);
		m_processingThread = std::make_unique<std::thread>(&ImageProcessor::process, this);
	}
}

void ImageProcessor::stop() {
	isRunning.store(false, std::memory_order_release);
}

void ImageProcessor::getOutput(nlohmann::json& output) const {
	std::shared_lock<std::shared_mutex> jsonLock(m_jsonLock);
	output = m_latestOutput;
}

void ImageProcessor::process() {
	cv::Mat img;
	unsigned int lastId = 0;
	while (isRunning.load(std::memory_order_acquire)) {
		unsigned int imgId = imgStream->read(img);
		if (lastId != imgId) {
			lastId = imgId;
			std::unique_lock<std::mutex> funcLock(m_funcLock);
			if (m_processingFunc) {
				nlohmann::json output = m_processingFunc(img);
				funcLock.unlock();
				std::unique_lock<std::shared_mutex> jsonLock(m_jsonLock);
				m_latestOutput = output;
			}
		} else {
			imgStream->waitForNextWrite(lastId);
		}
	}
}