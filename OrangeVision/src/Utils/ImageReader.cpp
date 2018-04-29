#include "ImageReader.hpp"

ImageReader::ImageReader(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream) {
	std::unique_lock<std::mutex> streamLock(m_streamLock);
	m_imgStream = imgStream;
}

ImageReader::~ImageReader() {
	stop();
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
void ImageReader::start() {
	if (m_processingThread) {
		if (!m_processingThread->joinable()) {
			isRunning.store(true, std::memory_order_release);
			m_processingThread = std::make_unique<std::thread>(&ImageReader::process, this);
		}
	} else {
		isRunning.store(true, std::memory_order_release);
		m_processingThread = std::make_unique<std::thread>(&ImageReader::process, this);
	}
}

void ImageReader::stop() {
	isRunning.store(false, std::memory_order_release);
}

void ImageReader::process() {
	while (isRunning.load(std::memory_order_acquire)) {
		std::unique_lock<std::mutex> streamLock(m_streamLock);
		m_imgStream->write(m_capture);
		/*
		cv::waitKey(10);
		*/
	}
}