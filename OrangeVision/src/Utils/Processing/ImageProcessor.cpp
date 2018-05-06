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
	unsigned int imgId = m_imgStream->read(m_img);
	streamLock.unlock();
	if (m_lastId != imgId) {
		m_lastId = imgId;
		std::unique_lock<std::mutex> funcLock(m_funcLock);
		if (m_processingFunc) {
			if (!m_img.empty()) {
				nlohmann::json output = m_processingFunc(m_img);
				funcLock.unlock();
				if (m_outputHandler) {
					m_outputHandler->update(output);
				}
			}
		}
	}
	else {
		streamLock.lock();
		m_imgStream->waitForNextWrite(m_lastId);
	}
}

