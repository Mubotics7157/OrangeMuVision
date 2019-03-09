#ifndef CALIBRATOR_HPP
#define CALIBRATOR_HPP

#include "Threading\ConcurrentStreamReader.hpp"
#include "Threading\ConcurrentStream.hpp"
#include <opencv2\opencv.hpp>

class Calibrator {
public:
	Calibrator(cv::Size size, double unit);
	bool detect(cv::Mat& img, bool add);
	void calibrate();
	bool saveTo(const std::string filename) const;
	static bool loadFrom(const std::string filename, cv::Mat& intrinsic, cv::Mat& distCoeffs);
private:
	cv::Mat m_intrinsic;
	cv::Mat m_distCoeffs;
	cv::Size m_boardSize;
	cv::Size m_imgSize;
	std::vector<cv::Point2f> m_cornerPoints;
	std::vector<cv::Point3f> m_chessPoints;
	std::vector<std::vector<cv::Point3f>> m_objectPoints;
	std::vector<std::vector<cv::Point2f>> m_imagePoints;
};

#endif