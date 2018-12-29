#include "AprilTagDetector.hpp"

AprilTagDetector::AprilTagDetector(AprilTags::TagCodes tagCode) : tagDetector(std::make_unique<AprilTags::TagDetector>(AprilTags::TagFamily(tagCode))) {
	tagDetector->segSigma = 0.5;
	tagDetector->sigma = 0;
	tagDetector->tagFamily.blackBorder = 1;
}

std::vector<AprilTagDetector::TagData> AprilTagDetector::detect(cv::Mat& img) {

	tagDetections = tagDetector->extractTags(img);
	for (int i = 0; i < tagDetections.size(); i++) {
		cv::Point center(tagDetections[i].cxy.first, tagDetections[i].cxy.second);
	}

	// Calculate position and rotations
	for (auto detection : tagDetections) {
		tagsData[detection.id] = detection;
	}

	std::vector<TagData> result;
	return result;
}

bool AprilTagDetector::getTagData(const unsigned int tagId, AprilTags::TagDetection& tagData) const {
	if (tagsData.find(tagId) != tagsData.end()) {
		tagData = tagsData.at(tagId);
		return true;
	} else {
		return false;
	}
}

AprilTagDetector::~AprilTagDetector() {

}

