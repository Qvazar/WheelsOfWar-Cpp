#include <algorithm>
#include <chrono>
#include <utility>
#include "Game.h"
#include "events/GameHaltEvent.h"
#include "Heartbeat.h"

using namespace std;

typedef chrono::steady_clock Clock;

namespace WheelsOfWarEngine {

Game::Game(const initializer_list<unique_ptr<Engine>>& engines)
: engines{engines}, isRunning{false}, updateHz{10} {

}

void Game::run() {
	this->isRunning = true;

	this->events.on<GameHaltEvent>([this](const GameHaltEvent&){ this->halt(); });

	for (auto& enginePtr : this->engines) {
		enginePtr->initialize(this->events);
	}

	auto hb = Heartbeat{0, 0f};
	auto lastTick = Clock::now();
	auto updateDelta = chrono::seconds(1f / this->updateHz);
	Clock::time_point lastUpdateTime;
	Clock::time_point now;

	while (this->isRunning) {
		now = Clock::now();

		hb.tick += 1;
		hb.deltaTime = now - lastTick;

		for (auto& enginePtr : this->engines) {
			enginePtr->tick(hb);
		}

		if (now - lastUpdateTime >= updateDelta) {
			for (auto& enginePtr : this->engines) {
				enginePtr->update(hb);
			}

			lastUpdateTime = now;
		}

		lastTick = move(now);
	}

	std::for_each(this->engines.rbegin(), this->engines.rend(), [](enginePtr&){
		enginePtr->shutdown();
	});
}

void Game::halt() {
	this->isRunning = false;
}

}