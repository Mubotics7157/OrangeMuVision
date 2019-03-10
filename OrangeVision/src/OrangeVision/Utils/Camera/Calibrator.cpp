#include "Calibrator.hpp"

namespace ov {
	Calibrator::Calibrator(cv::Size size, double unit) {
		for (int j = 0; j < size.height; j++) {
			for (int k = 0; k < size.width; k++)
				m_chessPoints.push_back(cv::Point3f(unit * j, unit * k, 0.0f));
		}
		m_boardSize = size;
	}

	bool Calibrator::detect(cv::Mat& img, bool add) {
		if (m_imgSize.empty()) {
			m_imgSize = img.size();
		}
		bool found = cv::findChessboardCorners(img, m_boardSize, m_cornerPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		if (found) {
			cv::drawChessboardCorners(img, m_boardSize, m_cornerPoints, found);
			if (add) {
				m_imagePoints.push_back(m_cornerPoints);
				m_objectPoints.push_back(m_chessPoints);
			}

		}

		return found;
	}

	void Calibrator::calibrate() {
		std::vector<cv::Mat> rvecs;
		std::vector<cv::Mat> tvecs;
		double rms = cv::calibrateCamera(m_objectPoints, m_imagePoints, m_imgSize, m_intrinsic, m_distCoeffs, rvecs, tvecs);
		std::cout << "Reprojection error: " << rms << std::endl;
	}

	bool Calibrator::saveTo(const std::string filename) const {
		cv::FileStorage file(filename, cv::FileStorage::WRITE);
		if (file.isOpened()) {
			file << "intrinsic" << m_intrinsic;
			file << "distCoeffs" << m_distCoeffs;
			return true;
		}
		return false;
	}

	bool Calibrator::loadFrom(const std::string filename, cv::Mat& intrinsic, cv::Mat& distCoeffs) {
		cv::FileStorage file(filename, cv::FileStorage::READ);
		if (file.isOpened()) {
			file["intrinsic"] >> intrinsic;
			file["distCoeffs"] >> distCoeffs;
			return true;
		}
		//Failed to open file
		return false;
	}

}
