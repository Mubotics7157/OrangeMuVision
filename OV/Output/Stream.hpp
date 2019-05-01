#include <opencv2/opencv.hpp>
#include "Threading/ConcurrentStreamReader.hpp"
#include "Threading/Updateable.hpp"
#include <mutex>
#include <iostream>

namespace ov {
  class Stream : public Updateable {
  public:
    Stream(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream, const char *pipe) 
    : m_inputStream(inputStream) {
      writer.open(pipe, 0, 90.0, cv::Size(320, 240), true);
    }
    
		void setInputStream(std::shared_ptr<ConcurrentStream<cv::Mat>> inputStream) {
			std::unique_lock<std::mutex> inputLock(m_inputLock);
			m_inputStream.setStream(inputStream);
		}
   
    void update() {      
			std::unique_lock<std::mutex> inputLock(m_inputLock);
			m_inputStream.read(m_data);
			inputLock.unlock();
      if(!m_data.empty()) {      
        writer.write(m_data);
      }
    }
    
  private:
		cv::Mat m_data;
		ConcurrentStreamReader<cv::Mat> m_inputStream;
    cv::VideoWriter writer;  
		std::mutex m_inputLock;
  };


}
