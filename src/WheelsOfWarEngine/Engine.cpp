#include "Engine.h"

namespace WheelsOfWar {

void Engine::initialize(EventBus& eventBus) {
	this->eventsPtr = &eventBus;
	this->onInitialize();
}

void Engine::shutdown() {
	this->onShutdown();
	this->eventsPtr = nullptr;
}

void Engine::update(const Heartbeat& hb) {

}

void Engine::tick(const Heartbeat& hb) {

}

Game& Engine::game() final const {
	return *gamePtr;
}

EventBus& Engine::events() final const {
	return *gamePtr->events();
}

} /* namespace WheelsOfWar */
