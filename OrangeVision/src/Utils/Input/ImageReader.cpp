#include "ImageReader.hpp"

ImageReader::ImageReader(std::shared_ptr<ConcurrentMat> imgStream, std::shared_ptr<cv::VideoCapture> capture) {
	std::unique_lock<std::mutex> captureLock(m_streamLock);
	m_capture = capture;
	captureLock.unlock();
	std::unique_lock<std::mutex> streamLock(m_streamLock);
	m_imgStream = imgStream;
	streamLock.unlock();

}

ImageReader::~ImageReader() {

}

void ImageReader::setStream(std::shared_ptr<ConcurrentMat> imgStream) {
	std::unique_lock<std::mutex> streamLock(m_streamLock);
	m_imgStream = imgStream;
}

void ImageReader::setCapture(std::shared_ptr<cv::VideoCapture>) {
	
}

void ImageReader::update() {
	std::unique_lock<std::mutex> captureLock(m_captureLock);
	if (m_capture->isOpened()) {
		if (m_capture->grab()) {
			m_capture->retrieve(m_imgBuffer);
			captureLock.unlock();
			std::unique_lock<std::mutex> streamLock(m_streamLock);
			m_imgStream->write(m_imgBuffer);
		} else {
			m_capture->release();
		}
	}
}