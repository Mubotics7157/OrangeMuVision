#pragma once
#ifndef FAKE_HANDLER_HPP
#define FAKE_HANDLER_HPP
#include <iostream>
#include <nholmann\json.hpp>
#include "OutputHandler.hpp"

class FakeHandler : public OutputHandler {
public:
	FakeHandler(std::shared_ptr<ConcurrentJson> inputStream) : OutputHandler(inputStream) {
	}

	void process(nlohmann::json& json) {
		std::cout << json.dump(4) << std::endl;
	}
};


#endif