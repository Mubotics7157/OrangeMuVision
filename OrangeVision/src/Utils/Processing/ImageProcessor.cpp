#include "ImageProcessor.hpp"
#include <chrono>

ImageProcessor::ImageProcessor(std::shared_ptr<ConcurrentMat> inputStream, std::shared_ptr<ConcurrentJson> outputStream) {
	m_inputStream = inputStream;
	m_outputStream = outputStream;
}

void ImageProcessor::setInputStream(std::shared_ptr<ConcurrentMat> imgStream) {
	std::unique_lock<std::mutex> streamLock(m_inputLock);
	m_inputStream = imgStream;
}

void ImageProcessor::setOutputStream(std::shared_ptr<ConcurrentJson> outputStream) {
	std::unique_lock<std::mutex> outputLock(m_outputLock);
	m_outputStream = outputStream;
}


void ImageProcessor::update() {
	std::unique_lock<std::mutex> inputLock(m_inputLock);
	unsigned int imgId = m_inputStream->read(m_img);
	inputLock.unlock();
	if (m_lastId != imgId) {
		m_lastId = imgId;
		if (!m_img.empty()) {
			nlohmann::json output = process(m_img);
			std::unique_lock<std::mutex> outputLock(m_outputLock);
			m_outputStream->write(output);
		}
	} else {
		inputLock.lock();
		m_inputStream->waitForNextWrite(m_lastId);
	}
}