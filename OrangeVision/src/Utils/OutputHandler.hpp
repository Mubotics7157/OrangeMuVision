#pragma once
#ifndef OUTPUT_HANDLER_HPP
#define OUTPUT_HANDLER_HPP
#include <nholmann\json.hpp>

class OutputHandler {
public:
	virtual void update(nlohmann::json& json);
};

#endif