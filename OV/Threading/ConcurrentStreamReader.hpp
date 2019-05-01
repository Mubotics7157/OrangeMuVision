#pragma once
#ifndef CONCURRENT_STREAM_READER_HPP
#define CONCURRENT_STREAM_READER_HPP
#include "ConcurrentStream.hpp"
#include <memory>
#include <iostream>
namespace ov {
	//Ensures all reads will get the latest data or wait until there is new data
	template<typename T>
	class ConcurrentStreamReader {
	public:
		ConcurrentStreamReader(std::shared_ptr<ConcurrentStream<T>> inputStream) {
			m_inputStream = inputStream;
		}

		void read(T& data) {
			unsigned int id = m_inputStream->getId();
			if (m_lastId != id) {
				m_inputStream->read(data);
				m_lastId = id;
			}
			else {
				m_inputStream->waitForNextWrite(m_lastId);
        id = m_inputStream->getId();
				m_inputStream->read(data);
				m_lastId = id;
			}
		}

		void setStream(std::shared_ptr<ConcurrentStream<T>> inputStream) {
			m_inputStream = inputStream;
		}

	private:
		unsigned int m_lastId = 0;
		std::shared_ptr<ConcurrentStream<T>> m_inputStream;
	};
}
#endif