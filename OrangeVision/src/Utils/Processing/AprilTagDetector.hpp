#pragma once
#ifndef APRIL_TAG_DETECTOR_H_
#define APRIL_TAG_DETECTOR_H_
#include "AprilTags\include\TagFamily.h"
#include "AprilTags\include\TagDetector.h"
#include "AprilTags\include\TagDetection.h"
#include <vector>
#include <memory>

class AprilTagDetector {
	public:
		struct TagData {
			cv::Point tagLocation;
			std::vector<cv::Point2f> tagCorners;
		};
		struct DetectionData {
			int tagId;
			TagData tagInfo;
		};
		AprilTagDetector(AprilTags::TagCodes tagCode);
		void detect(cv::Mat& img);
		bool getTagData(const unsigned int tagId, TagData& tagData) const;
		~AprilTagDetector();

	private:
		void updateTagPositions(); 
		std::unique_ptr<AprilTags::TagDetector> tagDetector;
		std::vector<AprilTags::TagDetection> tagDetections;
		std::map<unsigned int, TagData> tagsData;
};

#endif