#pragma once
#ifndef UPDATEABLE_HPP
#define UPDATEABLE_HPP

namespace ov {
	class Updateable {
	public:
		virtual void update() = 0;
	};
}
#endif 