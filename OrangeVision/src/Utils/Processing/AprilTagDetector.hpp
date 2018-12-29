#pragma once
#ifndef APRIL_TAG_DETECTOR_H_
#define APRIL_TAG_DETECTOR_H_
#include "TagFamily.h"
#include "TagDetector.h"
#include "TagDetection.h"
#include <vector>
#include <memory>

class AprilTagDetector {
	public:
		struct TagData {
			//id
			//Position and Rotation
			int i;
		};
		AprilTagDetector(AprilTags::TagCodes tagCode);
		std::vector<TagData> detect(cv::Mat& img);
		bool getTagData(const unsigned int tagId, AprilTags::TagDetection& tagData) const;
		~AprilTagDetector();

	private:
		std::unique_ptr<AprilTags::TagDetector> tagDetector;
		std::vector<AprilTags::TagDetection> tagDetections;
		std::vector<int> tagsDetected;
		std::map<unsigned int, AprilTags::TagDetection> tagsData;
};

#endif