#pragma once

#include <chrono>

namespace WheelsOfWarEngine {

struct Heartbeat {
	unsigned long tick;
	std::chrono::seconds deltaTime;
	bool doUpdate;
};

} /* namespace WheelsOfWar */
