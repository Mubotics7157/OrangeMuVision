#pragma once
#ifndef CONCURRENT_JSON_HPP
#define CONCURRENT_JSON_HPP
#include <nholmann\json.hpp>
#include <shared_mutex>
#include <atomic>

class ConcurrentJson {
public:
	void write(nlohmann::json& json);
	int read(nlohmann::json& json) const;
	void waitForNextWrite(unsigned int currentJsonId);
private:
	mutable std::shared_mutex m_jsonLock;
	mutable std::mutex m_signalLock;
	nlohmann::json m_lastJson;
	std::atomic<unsigned int> m_jsonId;
	std::condition_variable signalNew;

};

#endif