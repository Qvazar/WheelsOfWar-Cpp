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
#include "Component.h"
#include "UpdateEvent.h"

namespace WheelsOfWar {

class Entity {
public:
	Entity(const std::initializer_list<Component*>&);
	virtual ~Entity();

	virtual void update(const UpdateEvent&);
	void addChildren(const std::initializer_list<Entity>&);
	void removeChildren(const std::initializer_list<Entity>&);

private:
	const std::forward_list<Component*> components;
};

} /* namespace WheelsOfWar */

#endif /* SRC_ENTITY_H_ */
