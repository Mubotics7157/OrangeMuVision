#ifndef CALIBRATION_UTIL_HPP
#define CALIBRATION_UTIL_HPP

#include "Threading\Updateable.hpp"
#include "Threading\ConcurrentStream.hpp"
#include "Calibrator.hpp"

namespace ov {
	class CalibrationUtil : public Updateable {
	public:
		CalibrationUtil(int limit, cv::Size size, double unit, std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream);
		Calibrator getCalib();
		void update();
	private:
		cv::Mat m_data;
		bool m_last;
		int m_limit;
		int m_success;
		ConcurrentStreamReader<cv::Mat> m_inputStream;
		Calibrator m_calibrator;
	};

}
#endif