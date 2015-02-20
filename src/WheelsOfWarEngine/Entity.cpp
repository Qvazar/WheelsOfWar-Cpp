#include "Entity.h"

namespace WheelsOfWarEngine {

void Entity::update(const Heartbeat& hb) noexcept {
	for (const auto& p : this->components) {
		p->second->update(hb);
	}

	for (const auto& p : this->entities) {
		p->second->update(hb);
	}
}

void Entity::applyUpdate() noexcept {
	for (const auto& p : this->components) {
		p->second->applyUpdate();
	}

	for (const auto& p : this->entities) {
		p->second->applyUpdate();
	}
}

void Entity::tick(const Heartbeat& hb) noexcept {
	for (const auto& p : this->components) {
		p->second->tick(hb);
	}

	for (const auto& p : this->entities) {
		p->second->tick(hb);
	}
}

}