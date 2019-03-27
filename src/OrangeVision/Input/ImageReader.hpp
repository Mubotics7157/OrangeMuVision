#pragma once
#ifndef IMAGE_READER_HPP
#define IMAGE_READER_HPP
#include <memory>
#include <mutex>
#include <opencv2\opencv.hpp>
#include "Threading\ConcurrentStream.hpp"
#include "Threading\Updateable.hpp"

namespace ov {
	class ImageReader : public Updateable {
	public:
		ImageReader(std::shared_ptr<cv::VideoCapture> capture);
		std::shared_ptr<ConcurrentStream<cv::Mat>> getImgStream();
		void setCapture(std::shared_ptr<cv::VideoCapture> capture);
		void update();
	private:
		std::mutex m_captureLock;
		std::shared_ptr<ConcurrentStream<cv::Mat>> m_imgStream;
		std::shared_ptr<cv::VideoCapture> m_capture;
		cv::Mat m_imgBuffer;

	};
}
#endif