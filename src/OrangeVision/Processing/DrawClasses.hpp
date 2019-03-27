#pragma once
#ifndef OV_DRAW_CLASSES_HPP
#define OV_DRAW_CLASSES_HPP

#include "Threading\Updateable.hpp"
#include "Threading\ConcurrentStreamReader.hpp"
#include <opencv2\opencv.hpp>

namespace ov {
	class DrawClasses : public Updateable {
	public:

	private:
		ConcurrentStreamReader<cv::Mat> m_inputStream;
	};


}

#endif