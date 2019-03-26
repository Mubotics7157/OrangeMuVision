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
		cv::Mat gray;
		cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
		bool found = cv::findChessboardCorners(gray, m_boardSize, m_cornerPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		if (found) {
			if (add) {
				cv::cornerSubPix(gray, m_cornerPoints, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 10, 0.01));
				m_imagePoints.push_back(m_cornerPoints);
				m_objectPoints.push_back(m_chessPoints);
			}
			cv::drawChessboardCorners(img, m_boardSize, m_cornerPoints, found);

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
