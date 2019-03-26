#pragma once
#ifndef OV_DISPLAY_HPP
#define OV_DISPLAY_HPP
#include <memory>
#include "Threading\Updateable.hpp"
#include "Threading\ConcurrentStream.hpp"
#include "Threading\ConcurrentStreamReader.hpp"

namespace ov {
	class Display : public Updateable {
	public:

	private:
		std::shared_ptr<ConcurrentStream<char>>  m_outputStream;
		ConcurrentStreamReader<cv::Mat> m_inputStream;
		

	};

}

#endif