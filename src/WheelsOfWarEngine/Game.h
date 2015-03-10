#pragma once

#include <initializer_list>
#include <memory>
#include <array>
#include "Engine.h"
#include "EventBus.h"

using std;

namespace WheelsOfWarEngine {

class Game final {
public:
	Game(const initializer_list<unique_ptr<Engine>>&);
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	~Game() = default;

	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;

	void run();
	void halt();

	EventBus& events() const { return events; }

private:
	array<unique_ptr<Engine>> engines;
	bool isRunning;
	char updateHz;
	EventBus events;
};

}