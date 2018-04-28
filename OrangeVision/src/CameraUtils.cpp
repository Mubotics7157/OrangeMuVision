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

	void ConcurrentMat::write(cv::VideoCapture& capture) {
		if (capture.isOpened()) {
			if (capture.grab()) {
				std::unique_lock<std::shared_mutex> imgLock(m_imgLock);
				capture.retrieve(m_lastImg);
				imgLock.unlock();
				std::unique_lock<std::mutex> signalLock(m_signalLock);
				++m_imgId;
				signalLock.unlock();
				signalNew.notify_all();
			} else {
				capture.release();
			}
		}
	}

	int ConcurrentMat::read(cv::Mat& image) const {
		std::shared_lock<std::shared_mutex> lock(m_imgLock);
		image = m_lastImg.clone();
		return m_imgId;
	}

	void ConcurrentMat::waitForNextWrite(unsigned int currentImgId) {
		std::cout << "waiting" << std::endl;
		std::unique_lock<std::mutex> lock(m_signalLock);
		signalNew.wait(lock, [&currentImgId, this](){return currentImgId != m_imgId;});
	}
}
