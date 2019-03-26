#pragma once
#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP
#include <mutex>
#include "Threading\Updateable.hpp"
#include "Threading\ConcurrentStreamReader.hpp"
#include "Processing\AprilTagDetector.hpp"
#include "Utils\TextToPoints.hpp"
#include "Utils\IdToClass.hpp"
#include "nholmann\json.hpp"
#include "Tag36h15.h"

namespace ov {
	class Localization : public Updateable {
	public:
		//Only takes in a stream of cv::Mat
		Localization(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream, cv::Mat intrins, cv::Mat distCoeff)
			: m_inputStream(inputStream), m_detector(AprilTags::tagCodes36h15, intrins, distCoeff) {
			m_outputStream = std::make_shared<ConcurrentStream<nlohmann::json>>();
			m_intrins.write(intrins);
			m_distCoeff.write(distCoeff);
		}

		nlohmann::json process() {
			std::ifstream ifs("class_data.json");
			nlohmann::json json;
			ifs >> json;
			cv::Mat intrins, distCoeff;
			m_intrins.read(intrins);
			m_distCoeff.read(distCoeff);
			if (!m_data.empty()) {
				cv::Mat gray;
				cv::cvtColor(m_data, gray, cv::COLOR_BGR2GRAY);
				std::vector<AprilTagDetector::TagData> tagDatas = m_detector.detect(gray);
				for (auto tagData : tagDatas) {
					std::vector<cv::Point2f> corners;
					for (int i = 0; i < 4; ++i) {
						corners.push_back(cv::Point2f(tagData.tagDetection.p[i].first, tagData.tagDetection.p[i].second));
						cv::line(m_data, cv::Point(tagData.tagDetection.p[i].first, tagData.tagDetection.p[i].second), cv::Point(tagData.tagDetection.p[(i + 1) % 4].first, tagData.tagDetection.p[(i + 1) % 4].second), cv::Scalar(0, 0, i * 80), 1);
					}
					cv::Mat R;
					cv::Rodrigues(tagData.rvecs, R);
					R = R.t();
					cv::Mat invTvec = -R * tagData.tvecs;
					std::cout << invTvec.cols << " " << invTvec.rows << std::endl;
					for (int i = 0; i < invTvec.rows; ++i) {
						std::cout << invTvec.at<float>(0, i) << " ";
					}
					std::cout << std::endl;
				}
				cv::resize(m_data, m_data, cv::Size(0, 0), 0.5, 0.5);
				cv::imshow("test", m_data);
				char key = cv::waitKey(1);
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

		void updateIntrins(cv::Mat intrins) {
			m_intrins.write(intrins);
		}

		void updateDistCoeff(cv::Mat distCoeff) {
			m_distCoeff.write(distCoeff);
		}

	private:
		cv::Mat m_data;
		std::mutex m_inputLock;
		ConcurrentStreamReader<cv::Mat> m_inputStream;
		std::shared_ptr<ConcurrentStream<nlohmann::json>> m_outputStream;
		AprilTagDetector m_detector;
		ConcurrentStream<cv::Mat> m_intrins;
		ConcurrentStream<cv::Mat> m_distCoeff;
		int m_class = 0;
	};
}
#endif