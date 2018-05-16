#include "ImageReader.hpp"
#include "CameraUtils.hpp"

ImageReader::ImageReader(std::shared_ptr<cv::VideoCapture> capture) {
	std::unique_lock<std::mutex> captureLock(m_captureLock);
	m_capture = capture;
	captureLock.unlock();
	//cv::Mat operator = does a shallow copy
	m_imgStream = std::make_shared<ConcurrentStream<cv::Mat>>(&CameraUtils::copyMat);
}

std::shared_ptr<ConcurrentStream<cv::Mat>> ImageReader::getImgStream() {
	return m_imgStream;
}

void ImageReader::setCapture(std::shared_ptr<cv::VideoCapture> capture) {
	std::unique_lock<std::mutex> captureLock(m_captureLock);
	m_capture = capture;
}

void ImageReader::update() {
	std::unique_lock<std::mutex> captureLock(m_captureLock);
	if (m_capture->isOpened()) {
		if (m_capture->grab()) {
			m_capture->retrieve(m_imgBuffer);
			captureLock.unlock();
			m_imgStream->write(m_imgBuffer);
		} else {
			m_capture->release();
		}
	}
}