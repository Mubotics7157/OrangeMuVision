#ifndef UNDISTORT_HPP
#define UNDISTORT_HPP

#include "Utils\Threading\Updateable.hpp"
#include "Utils\Threading\ConcurrentStream.hpp"
#include "Utils\Threading\ConcurrentStreamReader.hpp"
#include <opencv2\opencv.hpp>
#include <memory>

class Undistort : public Updateable {
public:
	Undistort(cv::Mat intrins, cv::Mat distCoeffs, std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream);
	std::shared_ptr<ConcurrentStream<cv::Mat>> getImgStream();
	void update();
private:
	ConcurrentStreamReader<cv::Mat> m_inputStream;
	std::shared_ptr<ConcurrentStream<cv::Mat>> m_imgStream;
	cv::Mat m_data;
	cv::Mat m_undistorted;
	cv::Mat m_intrins;
	cv::Mat m_distCoeffs;
};

#endif