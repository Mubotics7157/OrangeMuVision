#pragma once
#ifndef IMAGE_READER_H
#define IMAGE_READER_H
#include <atomic>
#include <memory>
#include <thread>
#include <mutex>
#include "Utils\Camera\ConcurrentMat.hpp"
#include "Utils\Threading\Updateable.hpp"

class ImageReader : public Updateable {
public:
	ImageReader(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream, std::shared_ptr<cv::VideoCapture> capture);
	~ImageReader();
	void setStream(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream);
	void setCapture(std::shared_ptr<cv::VideoCapture> capture);
	void update();
private:
	std::mutex m_streamLock;
	std::mutex m_captureLock;
	std::condition_variable m_signalWrite;
	std::shared_ptr<CameraUtils::ConcurrentMat> m_imgStream;
	std::shared_ptr<cv::VideoCapture> m_capture;
	cv::Mat m_imgBuffer;

};


#endif