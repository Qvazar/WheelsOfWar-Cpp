#include "Engine.h"
#include "thread-util.h"

namespace WheelsOfWar {

using namespace std;

void Engine::initialize(Game& game) {
	this->gamePtr = &game;
	this->onInitialize();

	this->run();
}

void Engine::shutdown() {
	this->onShutdown();
	this->eventsPtr = nullptr;

	this->running = false;
	heartbeatsCv.notify_all();
	runThread.join();
}

void Engine::run() {
	this->running = true;

	runThread = thread([this]() {
		mutex m;
		unique_lock<mutex> lk(m);

		while (this->running) {
			if (!heartbeats.empty()) {
				Heartbeat hb = lock(heartbeatsMutex, [this]() {
					return this->heartbeats.pop();
				});

				this->doTick(hb);
				if (hb.doUpdate) {
					this->doUpdate(hb);
				}
			}

			heartbeatsCv.wait(lk, [this]() {
				return !this->heartbeats.empty() || !this->running;
			});
		}
	});
}

void Engine::tick(const Heartbeat& hb) {
	lock(heartbeatsMutex, [&hb]() {
		this->heartbeats.push(hb);
		cv.notify_all();
	});
}

Game& Engine::game() final const {
	return *gamePtr;
}

EventBus& Engine::events() final const {
	return *gamePtr->events();
}

} /* namespace WheelsOfWar */
