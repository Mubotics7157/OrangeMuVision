#include "ConcurrentJson.hpp"


void ConcurrentJson::write(nlohmann::json& json) {
	std::unique_lock<std::shared_mutex> jsonLock(m_jsonLock);
	m_lastJson = json;
	jsonLock.unlock();
	m_jsonId.fetch_add(1, std::memory_order_release);
	signalNew.notify_all();
}

int ConcurrentJson::read(nlohmann::json& json) const {
	std::shared_lock<std::shared_mutex> jsonLock(m_jsonLock);
	json = m_lastJson;
	return m_jsonId.load(std::memory_order_acquire);
}

void ConcurrentJson::waitForNextWrite(unsigned int currentJsonId) {
	std::unique_lock<std::mutex> lock(m_signalLock);
	signalNew.wait_for(lock, std::chrono::milliseconds(200), [&currentJsonId, this]() {return currentJsonId != m_jsonId.load(std::memory_order_acquire); });
}