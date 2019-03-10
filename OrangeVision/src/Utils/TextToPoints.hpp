#pragma once
#ifndef TEXT_TO_POINTS_HPP
#define TEXT_TO_POINTS_HPP
#include <vector>
#include <string.h>
#include "opencv2/opencv.hpp"

namespace OrangeVision {

	std::vector<cv::Point3f> textToPoints(std::string text, int xOffset, int yOffset) {	
		double fontScale = 4;
		cv::Size textSize = cv::getTextSize(text, 1, fontScale, 1, 0);
		int pad = 20;
		cv::Mat img = cv::Mat::zeros(cv::Size(yOffset + textSize.width, xOffset + textSize.height + pad), CV_8UC1);
		cv::putText(img, text, cv::Point(yOffset, xOffset + textSize.height + pad / 2), 1, fontScale, cv::Scalar(255), 1, 8);
		std::vector<cv::Point3f> points;
		for (int i = 0; i < img.rows; ++i) {
			for (int j = 0; j < img.cols; ++j) {
				if (img.at<uchar>(i, j) == 255) {
					points.push_back(cv::Point3f(float(i) / 5, float(j) / 5 - 60, 20));
				}
			}
		}
		return points;
	}

}

#endif