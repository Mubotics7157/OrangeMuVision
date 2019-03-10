#include "Undistort.hpp"

namespace ov {
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
			cv::undistort(m_data, m_undistorted, m_intrins, m_distCoeffs);
			m_imgStream->write(m_undistorted);
		}

	}
}