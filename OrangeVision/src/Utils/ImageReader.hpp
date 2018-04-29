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
	void start();
	void stop();
private:
	void process();
	std::mutex m_streamLock;
	std::atomic<bool> isRunning = false;
	std::shared_ptr<CameraUtils::ConcurrentMat> m_imgStream;
	std::unique_ptr<std::thread> m_processingThread{ nullptr };
	cv::VideoCapture m_capture;

};


#endif