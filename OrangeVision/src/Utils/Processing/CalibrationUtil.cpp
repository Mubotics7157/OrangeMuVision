#include "CalibrationUtil.hpp"

CalibrationUtil::CalibrationUtil(int limit, cv::Size size, double unit, std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream) : m_inputStream(inputStream), m_calibrator(size, unit) {
	m_limit = limit;
	m_success = 0;
}

Calibrator CalibrationUtil::getCalib() {
	return m_calibrator;
}

void CalibrationUtil::update() {
	m_inputStream.read(m_data);
	if (!m_data.empty()) {
		char key = cv::waitKey(1);
		if (m_limit < 1) {
			if (key == 'k') {
				if (!m_last) {
					if (m_calibrator.detect(m_data, true)) {
						++m_success;
						std::cout << m_success << std::endl;
					}
				}
				m_last = true;
			} else {
				m_calibrator.detect(m_data, false);				
				m_last = false;
			}
			if (key == 'c') {
				m_calibrator.calibrate();
				m_calibrator.saveTo("config.txt");
			}
		} else {
			if (m_limit > m_success) {
				if (m_calibrator.detect(m_data, true)) {
					++m_success;
				}
				if (m_limit == m_success) {
					m_calibrator.calibrate();
					m_calibrator.saveTo("config.txt");
				}
			} else {
				m_calibrator.detect(m_data, false);
			}
		}
		cv::imshow("Calibration", m_data);
	}
}