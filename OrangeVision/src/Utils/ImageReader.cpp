#include "ImageReader.hpp"

ImageReader::ImageReader(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream) {
	std::unique_lock<std::mutex> streamLock(m_streamLock);
	m_imgStream = imgStream;
	isRunning.store(true, std::memory_order_release);
	m_processingThread = std::make_unique<std::thread>(&ImageReader::process, this);
}

ImageReader::~ImageReader() {
	release();
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
	isWriting.store(true, std::memory_order_release);
	m_signalWrite.notify_all();
}

void ImageReader::stop() {
	isWriting.store(false, std::memory_order_release);
}

void ImageReader::release() {
	isRunning.store(false, std::memory_order_release);
}

//TODO: attempt to restart camera
void ImageReader::process() {
	while (isRunning.load(std::memory_order_acquire)) {
		if (isWriting.load(std::memory_order_acquire)) {
			if (m_capture.isOpened()) {
				if (m_capture.grab()) {
					m_capture.retrieve(m_imgBuffer);
					m_imgStream->write(m_imgBuffer);
				}
				else {
					m_capture.release();
				}
			}
		} else {
			std::unique_lock<std::mutex> lock(m_signalLock);
			m_signalWrite.wait_for(lock, std::chrono::milliseconds(200), [this]() {return isWriting.load(std::memory_order_acquire);});
		}
	}
}