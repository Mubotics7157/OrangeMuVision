#include "AprilTagDetector.hpp"

AprilTagDetector::AprilTagDetector(AprilTags::TagCodes tagCode, cv::Mat intrins, cv::Mat distCoeff)
: tagDetector(std::make_unique<AprilTags::TagDetector>(tagCode)), m_intrins(intrins), m_distCoeff(distCoeff) {
	tagDetector->SetSegmentationSigma(0.5);
	tagDetector->SetSigma(0);
}

std::vector<AprilTagDetector::TagData> AprilTagDetector::detect(cv::Mat& img) {

	tagDetections = tagDetector->extractTags(img);
	for (int i = 0; i < tagDetections.size(); i++) {
		cv::Point center(tagDetections[i].cxy.first, tagDetections[i].cxy.second);
	}

	std::vector<TagData> result;
	// Calculate position and rotations
	for (auto detection : tagDetections) {
		TagData data;
		data.i = detection.id;
		data.tagDetection = detection;
		data.rvecs = cv::Mat();
		data.tvecs = cv::Mat();

		std::vector<cv::Point2f> corners;
		for (int i = 0; i < 4; ++i) {
			corners.push_back(cv::Point2f(detection.p[i].first, detection.p[i].second));
		}
		cv::cornerSubPix(img, corners, cv::Size(2, 2), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 10, 0.01));
		std::vector<cv::Point3f> obj;
		double size = 5 + 1.5/16.0;
		obj.push_back(cv::Point3f(-size / 2, -size / 2, 0));
		obj.push_back(cv::Point3f(size / 2, -size / 2, 0));
		obj.push_back(cv::Point3f(size / 2, size / 2, 0));
		obj.push_back(cv::Point3f(-size / 2, size / 2, 0));
		std::vector<cv::Point2f> imgPoints;

		cv::solvePnP(obj, corners, m_intrins, m_distCoeff, data.rvecs, data.tvecs);
		tagsData[detection.id] = data;
		result.push_back(data);
	}

	return result;
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

