#include "ConcurrentMat.hpp"

namespace CameraUtils {
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
			}
			else {
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
		std::unique_lock<std::mutex> lock(m_signalLock);
		signalNew.wait_for(lock, std::chrono::milliseconds(200), [&currentImgId, this]() {return currentImgId != m_imgId;});
	}
}