#include "Undistort.hpp"

Undistort::Undistort(cv::Mat intrins, cv::Mat distCoeffs, std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream) : m_inputStream(inputStream) {
	m_intrins = intrins;
	m_distCoeffs = distCoeffs;
	m_imgStream = std::make_shared<ConcurrentStream<cv::Mat>>();
}

std::shared_ptr<ConcurrentStream<cv::Mat>> Undistort::getImgStream() {
	return m_imgStream;
}

void Undistort::update() {
	m_inputStream.read(m_data);
	if (!m_data.empty()) {
		//cv::imshow("Orig", m_data);
		cv::undistort(m_data, m_undistorted, m_intrins, m_distCoeffs);
		//cv::imshow("Undistorted", m_undistorted);
		m_imgStream->write(m_undistorted);
		//cv::waitKey(1);
	}

}