#include "ImageProcessor.hpp"
#include <chrono>

ImageProcessor::ImageProcessor(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream) : m_imgStream(imgStream) {
}

ImageProcessor::~ImageProcessor() {
}

void ImageProcessor::setStream(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream) {
	std::unique_lock<std::mutex> streamLock(m_streamLock);
	m_imgStream = imgStream;
}

void ImageProcessor::setProcessingFunction(std::function<nlohmann::json(cv::Mat& img)> processingFunc) {
	std::unique_lock<std::mutex> lock(m_funcLock);
	m_processingFunc = processingFunc;
}

void ImageProcessor::getOutput(nlohmann::json& output) const {
	std::shared_lock<std::shared_mutex> jsonLock(m_jsonLock);
	output = m_latestOutput;
}

void ImageProcessor::update() {
	std::unique_lock<std::mutex> streamLock(m_streamLock);
	unsigned int imgId = m_imgStream->read(img);
	streamLock.unlock();
	if (lastId != imgId) {
		lastId = imgId;
		std::unique_lock<std::mutex> funcLock(m_funcLock);
		if (m_processingFunc) {
			if (!img.empty()) {
				nlohmann::json output = m_processingFunc(img);
				funcLock.unlock();
				std::unique_lock<std::shared_mutex> jsonLock(m_jsonLock);
				m_latestOutput = output;
			}
		}
	} else {
		streamLock.lock();
		m_imgStream->waitForNextWrite(lastId);
	}
}

