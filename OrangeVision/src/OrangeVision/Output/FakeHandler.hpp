#pragma once
#ifndef FAKE_HANDLER_HPP
#define FAKE_HANDLER_HPP
#include <iostream>
#include <nholmann\json.hpp>
#include "Threading\Updateable.hpp"
#include "Threading\ConcurrentStream.hpp"
#include "Threading\ConcurrentStreamReader.hpp"

namespace ov {
	class FakeHandler : public Updateable {
	public:
		FakeHandler(std::shared_ptr<ConcurrentStream<nlohmann::json>> inputStream) : m_inputStream(inputStream) {
		}

		void process(nlohmann::json& json) {
			std::cout << json.dump(4) << std::endl;
		}

		void update() {
			m_inputStream.read(m_data);
			process(m_data);
		}
	private:
		ConcurrentStreamReader<nlohmann::json> m_inputStream;
		nlohmann::json m_data;
	};

}
#endif