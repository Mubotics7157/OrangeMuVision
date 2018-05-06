#pragma once
#ifndef CONCURRENT_MAT_H
#define CONCURRENT_MAT_H
#include <opencv2\opencv.hpp>
#include <shared_mutex>

namespace CameraUtils {
	class ConcurrentMat {
	public:
		enum class WriteStatus {
			WRITE_SUCCESS = 0,
			GRAB_FAILED = 1,
			CAMERA_CLOSED = 2			
		};
		void write(cv::Mat& image);
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