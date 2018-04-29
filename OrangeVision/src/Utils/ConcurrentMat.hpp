#pragma once
#ifndef CONCURRENT_MAT_H
#define CONCURRENT_MAT_H
#include <opencv2\opencv.hpp>
#include <shared_mutex>

namespace CameraUtils {
	class ConcurrentMat {
	public:
		void write(cv::VideoCapture& capture);
		int read(cv::Mat& image) const;
		void waitForNextWrite(unsigned int currentImgId);
	private:
		mutable std::shared_mutex m_imgLock;
		mutable std::mutex m_signalLock;
		cv::Mat m_lastImg;
		unsigned int m_imgId;
		std::condition_variable signalNew;
	};
}
#endif