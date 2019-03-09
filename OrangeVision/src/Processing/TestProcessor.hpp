#pragma once
#ifndef TEST_PROCESSOR_HPP
#define TEST_PROCESSOR_HPP
#include <mutex>
#include "Threading\Updateable.hpp"
#include "Threading\ConcurrentStreamReader.hpp"
#include "Processing\AprilTagDetector.hpp"
#include "Tag36h15.h"
#include "Utils/TextToPoints.hpp"
#include "Utils/IdToClass.hpp"
#include "nholmann/json.hpp"

class TestProcessor : public Updateable {
public:
	//Only takes in a stream of cv::Mat
	TestProcessor(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream, cv::Mat intrins, cv::Mat distCoeff)
	: m_inputStream(inputStream), m_detector(AprilTags::tagCodes36h15, intrins, distCoeff), m_intrins(intrins), m_distCoeff(distCoeff) {
		m_outputStream = std::make_shared<ConcurrentStream<nlohmann::json>>();
	}

	nlohmann::json process() {
		std::ifstream ifs("class_data.json");
		nlohmann::json json;
		ifs >> json;
		std::string location = IdToClass::Map[3];
		if (!m_data.empty()) {
			//cv::imshow("test", img);
			cv::Mat gray;
			cv::cvtColor(m_data, gray, cv::COLOR_BGR2GRAY);
			std::vector<AprilTagDetector::TagData> tagDatas = m_detector.detect(gray);
			for(auto tagData : tagDatas) {
				std::vector<cv::Point2f> corners;
				for (int i = 0; i < 4; ++i) {
					corners.push_back(cv::Point2f(tagData.tagDetection.p[i].first, tagData.tagDetection.p[i].second));
					cv::line(m_data, cv::Point(tagData.tagDetection.p[i].first, tagData.tagDetection.p[i].second), cv::Point(tagData.tagDetection.p[(i + 1) % 4].first, tagData.tagDetection.p[(i + 1) % 4].second), cv::Scalar(0, 0, i * 80), 1);
				}
				int xOffset = 0;
				int yOffset = 0;				
				std::string text(json[location][m_class].dump(2));
				if (m_updateConsole) {
					system("cls");
					std::cout << json[location][m_class].dump(2);
					m_updateConsole = false;
				}
				std::string line;
				std::stringstream ss(text);
				while (std::getline(ss, line)) {
					std::vector<cv::Point3f> textPoints = OrangeVision::textToPoints(line, 57 * xOffset, 700 * yOffset);
					std::vector<cv::Point2f> imgPoints;
					cv::projectPoints(textPoints, tagData.rvecs, tagData.tvecs, m_intrins, m_distCoeff, imgPoints);
					for (auto point : imgPoints) {
						cv::circle(m_data, point, 1, cv::Scalar(0, 255, 0));
					}
					if (xOffset == 28) {
						xOffset = 0;
						yOffset++;
					}
					xOffset++;
				}
				
			}
			cv::imshow("test", m_data);
			int key = cv::waitKey(1);
			if (key == 'a') {
				m_class--;
				m_updateConsole = true;
			} else if(key == 'd') {
				m_class++;
				m_updateConsole = true;
			}
			m_class = m_class % json[location].size();
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
	bool m_updateConsole = true;
	int m_class = 0;
};

#endif