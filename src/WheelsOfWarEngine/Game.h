#pragma once

#include <initializer_list>
#include <memory>
#include <vector>
#include "Engine.h"
#include "EventBus.h"
#include "SceneManager.h"

namespace WheelsOfWarEngine {

using namespace std;

class Engine;

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

	EventBus& events() { return eventBus; }

private:
	vector<unique_ptr<Engine>> engines;
	bool isRunning;
	char updateHz;
	EventBus events;
	SceneManager scenes;
};

}
