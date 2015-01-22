/*
 * Entity.h
 *
 *  Created on: Jan 16, 2015
 *      Author: sj
 */

#ifndef SRC_ENTITY_H_
#define SRC_ENTITY_H_

#include <initializer_list>
#include <forward_list>
#include <string>
#include "Component.h"
#include "Heartbeat.h"
#include "MessageBus.h"

namespace WheelsOfWar {

template<typename... ComponentTypes>
class Entity {
public:
	Entity(std::initializer_list<Entity> children);
	virtual ~Entity();

	void update(const Heartbeat&);

	void tick(const Heartbeat&);

	void addChild(const std::string& id, const Entity&);

	void removeChild(const std::string& id);

	void removeChild(const Entity&);

	template<typename EventType>
	void on(const MessageBus::Listener<EventType>&);

	template<typename EventType>
	void off(const MessageBus::Listener<EventType>&);

	template<typename EventType>
	void send(const EventType&);

private:
	const std::forward_list<unique_ptr<Component>> components;
	const MessageBus msgbus;
};

} /* namespace WheelsOfWar */

#include "Entity.tcc"

#endif /* SRC_ENTITY_H_ */
