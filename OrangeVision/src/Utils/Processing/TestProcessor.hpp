#pragma once
#ifndef TEST_PROCESSOR_HPP
#define TEST_PROCESSOR_HPP
#include <mutex>
#include "Utils\Threading\Updateable.hpp"
#include "Utils\Threading\ConcurrentStreamReader.hpp"
#include "Utils\Processing\AprilTagDetector.hpp"
#include "Tag36h15.h"

class TestProcessor : public Updateable {
public:
	//Only takes in a stream of cv::Mat
	TestProcessor(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream, cv::Mat intrins, cv::Mat distCoeff)
	: m_inputStream(inputStream), m_detector(AprilTags::tagCodes36h15), m_intrins(intrins), m_distCoeff(distCoeff) {
		m_outputStream = std::make_shared<ConcurrentStream<nlohmann::json>>();
	}

	nlohmann::json process() {

		nlohmann::json json;
		json["time"] = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
		if (!m_data.empty()) {
			//cv::imshow("test", img);
			cv::Mat gray;
			cv::cvtColor(m_data, gray, cv::COLOR_BGR2GRAY);
			m_detector.detect(gray);
			AprilTags::TagDetection tagData;
			m_detector.getTagData(3, tagData);
			std::vector<cv::Point2f> corners;
			for (int i = 0; i < 4; ++i) {
				corners.push_back(cv::Point2f(tagData.p[i].first, tagData.p[i].second));
				cv::line(m_data, cv::Point(tagData.p[i].first, tagData.p[i].second), cv::Point(tagData.p[(i + 1) % 4].first, tagData.p[(i + 1) % 4].second), cv::Scalar(0, 0, i * 80), 1);
			}
			cv::Mat rvecs, tvecs;
			std::vector<cv::Point3f> obj;
			obj.push_back(cv::Point3f(-30.75, -30.75, 0));
			obj.push_back(cv::Point3f(30.75, -30.75, 0));
			obj.push_back(cv::Point3f(30.75, 30.75, 0));
			obj.push_back(cv::Point3f(-30.75, 30.75, 0));
			std::vector<cv::Point2f> imgPoints;
			std::vector<cv::Point3f> axis;

			axis.push_back(cv::Point3f(-30.75, -30.75, 0));
			axis.push_back(cv::Point3f(30.75, -30.75, 0));
			axis.push_back(cv::Point3f(30.75, 30.75, 0));
			axis.push_back(cv::Point3f(-30.75, 30.75, 0));
			axis.push_back(cv::Point3f(-30.75, -30.75, 30.75));
			axis.push_back(cv::Point3f(30.75, -30.75, 30.75));
			axis.push_back(cv::Point3f(30.75, 30.75, 30.75));
			axis.push_back(cv::Point3f(-30.75, 30.75, 30.75));
			axis.push_back(cv::Point3f(0, 0, 0));
			axis.push_back(cv::Point3f(0, 0, 30.75));
			
			cv::solvePnP(cv::Mat(obj), cv::Mat(corners), m_intrins, m_distCoeff, rvecs, tvecs, false, CV_ITERATIVE);
			cv::projectPoints(axis, rvecs, tvecs, m_intrins, m_distCoeff, imgPoints);		
			cv::line(m_data, imgPoints[0], imgPoints[1], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[1], imgPoints[2], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[2], imgPoints[3], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[3], imgPoints[0], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[4], imgPoints[5], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[5], imgPoints[6], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[6], imgPoints[7], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[7], imgPoints[4], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[0], imgPoints[4], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[1], imgPoints[5], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[2], imgPoints[6], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[3], imgPoints[7], cv::Scalar(0, 255, 0));
			cv::line(m_data, imgPoints[8], imgPoints[9], cv::Scalar(0, 255, 0));
		
			cv::imshow("test", m_data);
			cv::waitKey(1);
		}		
		return json;
	}
	
	void setInputStream(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream) {
		std::unique_lock<std::mutex> inputLock(m_inputLock);
		m_inputStream.setStream(inputStream);
	}

	std::shared_ptr<ConcurrentStream<nlohmann::json>> getJsonStream() {
		return m_outputStream;
	}

	void update() {
		std::unique_lock<std::mutex> inputLock(m_inputLock);
		m_inputStream.read(m_data);
		inputLock.unlock();
		m_outputStream->write(process());
	}

private:
	cv::Mat m_data;
	std::mutex m_inputLock;
	ConcurrentStreamReader<cv::Mat> m_inputStream;
	std::shared_ptr<ConcurrentStream<nlohmann::json>> m_outputStream;
	AprilTagDetector m_detector;
	cv::Mat m_intrins;
	cv::Mat m_distCoeff;
};

#endif