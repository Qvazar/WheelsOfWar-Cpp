#pragma once

#include "Heartbeat.h"

namespace WheelsOfWarEngine {

class Component {
	friend class Entity;
public:
	Component() = default;
	virtual ~Component() = default;

	virtual void update(const Heartbeat&) = 0;
	virtual void applyUpdate(void) = 0;
	virtual void tick(const Heartbeat&) = 0;

protected:
	Entity* parent;
};

} /* namespace WheelsOfWar */
