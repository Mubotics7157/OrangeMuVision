#include "ConcurrentMat.hpp"

void ConcurrentMat::write(cv::Mat& image) {
	std::unique_lock<std::shared_mutex> imgLock(m_imgLock);
	m_lastImg = image.clone();
	imgLock.unlock();
	m_imgId.fetch_add(1, std::memory_order_release);
	signalNew.notify_all();
}

int ConcurrentMat::read(cv::Mat& image) const {
	std::shared_lock<std::shared_mutex> lock(m_imgLock);
	image = m_lastImg.clone();
	return m_imgId.load(std::memory_order_acquire);
}

void ConcurrentMat::waitForNextWrite(unsigned int currentImgId) {
	std::unique_lock<std::mutex> lock(m_signalLock);
	signalNew.wait_for(lock, std::chrono::milliseconds(200), [&currentImgId, this]() {return currentImgId != m_imgId.load(std::memory_order_acquire);});
}
