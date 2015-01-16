/*
 * Entity.cpp
 *
 *  Created on: Jan 16, 2015
 *      Author: sj
 */

#include "Entity.h"

namespace WheelsOfWar {

Entity::Entity(const std::initializer_list<Component*>& components)
: components(components) {
	// TODO Auto-generated constructor stub

}

Entity::~Entity() {
	// TODO Auto-generated destructor stub
}

void Entity::update(const UpdateEvent& event) {
}

void Entity::addChildren(const std::initializer_list<Entity>&) {
}

void Entity::removeChildren(const std::initializer_list<Entity>&) {
}

} /* namespace WheelsOfWar */
