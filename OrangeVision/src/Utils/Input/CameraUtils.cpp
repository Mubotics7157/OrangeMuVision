#include "CameraUtils.hpp"

namespace CameraUtils {

	void Calibrator::addImage(cv::Mat& img) {

	}


	void Calibrator::calibrate(cv::Size imgSize) {
		std::vector<cv::Mat> rvecs;
		std::vector<cv::Mat> tvecs;
		cv::calibrateCamera(m_objectPoints, m_imagePoints, imgSize, m_intrinsic, m_distCoeffs, rvecs, tvecs);
	}

	bool Calibrator::saveTo(char* filename) const {
		cv::FileStorage file(filename, cv::FileStorage::WRITE);
		if (file.isOpened()) {
			file << "intrinsic" << m_intrinsic;
			file << "distCoeffs" << m_distCoeffs;
			return true;
		}
		return false;
	}

	bool Calibrator::loadFrom(char* filename, cv::Mat& intrinsic, cv::Mat& distCoeffs) {
		cv::FileStorage file(filename, cv::FileStorage::READ);
		if (file.isOpened()) {
			file["intrinsic"] >> intrinsic;
			file["distCoeffs"] >> distCoeffs;
			return true;
		}
		//Failed to open file
		return false;
	}

	void copyMat(cv::Mat& toCopy, const cv::Mat& fromCopy) {
		toCopy = fromCopy.clone();
	}
}
