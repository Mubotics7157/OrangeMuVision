#pragma once
#ifndef TEST_PROCESSOR_HPP
#define TEST_PROCESSOR_HPP
#include <mutex>
#include "Utils\Threading\Updateable.hpp"
#include "Utils\Threading\ConcurrentStreamReader.hpp"

class TestProcessor : public Updateable {
public:
	TestProcessor(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream)
	: m_inputStream(inputStream) {
		m_outputStream = std::make_shared<ConcurrentStream<nlohmann::json>>();
	}

	nlohmann::json process(cv::Mat& img) {
		nlohmann::json json;
		json["time"] = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
		if (!img.empty()) {
			//cv::imshow("test", img);
			cv::waitKey(1);
		}		
		return json;
	}
	
	void setInputStream(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream) {
		std::unique_lock<std::mutex> inputLock(m_inputLock);
		m_inputStream.setStream(inputStream);
	}

	std::shared_ptr<ConcurrentStream<nlohmann::json>> getJsonOutput() {
		return m_outputStream;
	}

	void update() {
		std::unique_lock<std::mutex> inputLock(m_inputLock);
		m_inputStream.read(m_data);
		inputLock.unlock();
		m_outputStream->write(process(m_data));
	}

private:
	cv::Mat m_data;
	std::mutex m_inputLock;
	ConcurrentStreamReader<cv::Mat> m_inputStream;
	std::shared_ptr<ConcurrentStream<nlohmann::json>> m_outputStream;
};

#endif