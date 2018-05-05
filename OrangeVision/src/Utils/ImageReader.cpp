#include "ImageReader.hpp"

ImageReader::ImageReader(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream) {
	std::unique_lock<std::mutex> streamLock(m_streamLock);
	m_imgStream = imgStream;
}

ImageReader::~ImageReader() {
	m_capture.release();
}

void ImageReader::setStream(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream) {
	std::unique_lock<std::mutex> streamLock(m_streamLock);
	m_imgStream = imgStream;
}

void ImageReader::open(int id) {
	m_capture = cv::VideoCapture(id);
}

void ImageReader::open(const char* filename) {
	m_capture = cv::VideoCapture(filename);
}

//TODO: attempt to restart camera
void ImageReader::update() {
	if (m_capture.isOpened()) {
		if (m_capture.grab()) {
			m_capture.retrieve(m_imgBuffer);
			std::unique_lock<std::mutex> streamLock(m_streamLock);
			m_imgStream->write(m_imgBuffer);
		}
		else {
			m_capture.release();
		}
	}
}