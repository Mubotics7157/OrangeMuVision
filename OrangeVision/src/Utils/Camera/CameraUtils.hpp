#pragma once
#ifndef CAMERA_UTILS_H_
#define CAMERA_UTILS_H_
#include <vector>
#include <opencv2\opencv.hpp>
#include "Utils\Camera\ConcurrentMat.hpp"

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
}
#endif
