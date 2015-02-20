#include "Scene.h"

namespace WheelsOfWarEngine {

void Scene::update(const Heartbeat& hb) noexcept {
	for (const auto& p : this->entities) {
		p->second->update(hb);
	}
}

void Scene::applyUpdate() noexcept {
	for (const auto& p : this->entities) {
		p->second->applyUpdate();
	}
}

void Scene::tick(const Heartbeat& hb) noexcept {
	for (const auto& p : this->entities) {
		p->second->tick(hb);
	}
}

void Scene::setActive(bool active) {
	this->active = active;
}

void Scene::isActive() {
	return this->active;
}

}