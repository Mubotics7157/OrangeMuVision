#pragma once
#ifndef CAMERA_UTILS_H_
#define CAMERA_UTILS_H_
#include <vector>
#include <shared_mutex>
#include <opencv2\opencv.hpp>
#include <condition_variable>

namespace CameraUtils {
	class Calibrator {
	public:
		void addImage(cv::Mat& img);
		void calibrate(cv::Size imgSize);
		bool saveTo(char* filename) const;
		static bool loadFrom(char* filename, cv::Mat& intrinsic, cv::Mat& distCoeffs);
	private:
		cv::Mat m_intrinsic;
		cv::Mat m_distCoeffs;
		std::vector<std::vector<cv::Point3f>> m_objectPoints;
		std::vector<std::vector<cv::Point2f>> m_imagePoints;
	};

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
