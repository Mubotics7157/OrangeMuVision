#pragma once
#ifndef APRIL_TAG_DETECTOR_HPP
#define APRIL_TAG_DETECTOR_HPP
#include <vector>
#include <memory>
#include <opencv2\opencv.hpp>
#include "TagFamily.h"
#include "TagDetector.h"
#include "TagDetection.h"

class AprilTagDetector {
	public:
		struct TagData {
			//id
			//Position and Rotation
			int i;
			AprilTags::TagDetection tagDetection;
			cv::Mat rvecs, tvecs;
		};
		AprilTagDetector(AprilTags::TagCodes tagCode, cv::Mat intrins, cv::Mat distCoeff);
		std::vector<TagData> detect(cv::Mat& img);
		bool getTagData(const unsigned int tagId, TagData& tagData) const;
		~AprilTagDetector();
	private:
		std::unique_ptr<AprilTags::TagDetector> tagDetector;
		std::vector<AprilTags::TagDetection> tagDetections;
		std::vector<int> tagsDetected;
		std::map<unsigned int, TagData> tagsData;
		cv::Mat m_intrins;
		cv::Mat m_distCoeff;
};

#endif