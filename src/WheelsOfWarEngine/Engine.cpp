#include "Engine.h"
#include "thread-util.h"

namespace WheelsOfWarEngine {

void Engine::initialize(Game& game) {
	gamePtr = &game;
	this->onInitialize();
}

void Engine::shutdown() {
	this->onShutdown();
	this->gamePtr = nullptr;
}

void Engine::update(const Heartbeat& hb) {

}

void Engine::tick(const Heartbeat& hb) {
	this->onTick(hb);
}

Game& Engine::game() const {
	return *gamePtr;
}

EventBus& Engine::events() const {
	return gamePtr->events();
}

} /* namespace WheelsOfWar */
