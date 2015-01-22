/*
 * Engine.h
 *
 *  Created on: Jan 22, 2015
 *      Author: sj
 */

#ifndef SRC_ENGINE_H_
#define SRC_ENGINE_H_

#include "Heartbeat.h"
#include "MessageBus.h"

namespace WheelsOfWar {

class Engine {
public:
	Engine(const MessageBus* const msgbus) : msgbus(msgbus) {};
	virtual ~Engine() {};

	virtual void update(const Heartbeat&) = 0;
	virtual void tick(const Heartbeat&) = 0;

protected:
	const MessageBus* const msgbus;
};

} /* namespace WheelsOfWar */

#endif /* SRC_ENGINE_H_ */
