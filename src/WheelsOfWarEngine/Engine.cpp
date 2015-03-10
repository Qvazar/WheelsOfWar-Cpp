#include "Engine.h"
#include "thread-util.h"

namespace WheelsOfWar {

using namespace std;

void Engine::initialize(Game& game) {
	this->gamePtr = &game;
	this->onInitialize();
}

void Engine::shutdown() {
	this->onShutdown();
	this->gamePtr = nullptr;
}

void Engine::tick(const Heartbeat& hb) {
	this->onTick(hb);
}

Game& Engine::game() final const {
	return *gamePtr;
}

EventBus& Engine::events() final const {
	return gamePtr->events();
}

} /* namespace WheelsOfWar */
