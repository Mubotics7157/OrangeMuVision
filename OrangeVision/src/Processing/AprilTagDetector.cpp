#include "AprilTagDetector.hpp"

AprilTagDetector::AprilTagDetector(AprilTags::TagCodes tagCode) : tagDetector(std::make_unique<AprilTags::TagDetector>(AprilTags::TagFamily(tagCode))) {
	tagDetector->segSigma = 0;
	tagDetector->sigma = 0;
	tagDetector->tagFamily.blackBorder = 1;
}

void AprilTagDetector::detect(cv::Mat& img) {

	tagDetections = tagDetector->extractTags(img);
	for (int i = 0; i < tagDetections.size(); i++) {
		cv::Point center(tagDetections[i].cxy.first, tagDetections[i].cxy.second);
		cv::circle(img, center, 10, cv::Scalar(255));
	}
	updateTagPositions();
}

void AprilTagDetector::updateTagPositions() {
	for (auto detection : tagDetections) {
		std::vector<cv::Point2f> corners;
		corners.push_back(cv::Point2f(detection.p[0].first, detection.p[0].second));
		corners.push_back(cv::Point2f(detection.p[1].first, detection.p[1].second));
		corners.push_back(cv::Point2f(detection.p[2].first, detection.p[2].second));
		corners.push_back(cv::Point2f(detection.p[3].first, detection.p[3].second));
		TagData data = {cv::Point(detection.cxy.first, detection.cxy.second), corners};
		tagsData[detection.id] = data;
	}
}

bool AprilTagDetector::getTagData(const unsigned int tagId, TagData& tagData) const {
	if (tagsData.find(tagId) != tagsData.end()) {
		tagData = tagsData.at(tagId);
		return true;
	} else {
		return false;
	}
}

AprilTagDetector::~AprilTagDetector() {

}

