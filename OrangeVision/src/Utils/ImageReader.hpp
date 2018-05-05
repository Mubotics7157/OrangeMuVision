#pragma once
#ifndef IMAGE_READER_H
#define IMAGE_READER_H
#include <atomic>
#include <memory>
#include <thread>
#include <mutex>
#include "Utils\ConcurrentMat.hpp"

class ImageReader {
public:
	ImageReader(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream);
	~ImageReader();
	void setStream(std::shared_ptr<CameraUtils::ConcurrentMat> imgStream);
	void open(int id);
	void open(const char* filename);
	void update();
private:
	std::mutex m_streamLock;
	std::condition_variable m_signalWrite;
	std::shared_ptr<CameraUtils::ConcurrentMat> m_imgStream;
	cv::VideoCapture m_capture;
	cv::Mat m_imgBuffer;

};


#endif