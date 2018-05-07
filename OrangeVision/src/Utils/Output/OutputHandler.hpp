#pragma once
#ifndef OUTPUT_HANDLER_HPP
#define OUTPUT_HANDLER_HPP
#include <nholmann\json.hpp>
#include "Utils\Threading\ConcurrentJson.hpp"
#include "Utils\Threading\Updateable.hpp"

class OutputHandler : public Updateable {
public:
	OutputHandler(std::shared_ptr<ConcurrentJson> inputStream);

	virtual void process(nlohmann::json& json) =0;

	void update();

private:
	unsigned int m_lastId = 0;
	std::shared_ptr<ConcurrentJson> m_inputStream;
	nlohmann::json m_data;
	std::mutex m_inputLock;

};

#endif