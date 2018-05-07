#include "OutputHandler.hpp"

OutputHandler::OutputHandler(std::shared_ptr<ConcurrentJson> inputStream) {
	m_inputStream = inputStream;
}

void OutputHandler::update() {
	std::unique_lock<std::mutex> inputLock(m_inputLock);
	unsigned int id = m_inputStream->read(m_data);
	inputLock.unlock();
	if (m_lastId != id) {
		m_lastId = id;
		process(m_data);
	} else {
		inputLock.lock();
		m_inputStream->waitForNextWrite(m_lastId);
	}
}