#include <algorithm>
#include <chrono>
#include <future>
#include <utility>
#include "Game.h"
#include "events/GameHaltEvent.h"
#include "Heartbeat.h"

using namespace std;

typedef chrono::steady_clock Clock;

namespace WheelsOfWarEngine {

Game::Game(const initializer_list<unique_ptr<Engine>>& engines)
: engines{engines}, isRunning{false}, updateHz{10}, scenes{this->events} {

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
	vector<future> futures;

	while (this->isRunning) {
		now = Clock::now();

		hb.tick += 1;
		hb.deltaTime = now - lastTick;

		futures.resize(engines.size());
		transform(engines.begin(), engines.end(), futures.begin(), [&hb](const Engine& engine) {
			return async(launch::async, [&]() { engine.tick(hb) });
		});

		// Wait for all threads to complete their work
		for_each(futures.begin(), futures.end(), [](const auto& f) {
			f.get();
		});
		
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