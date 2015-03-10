#pragma once

#include <queue>
#include "EventBus.h"
#include "Game.h"
#include "Heartbeat.h"

namespace WheelsOfWar {

using namespace std;

class Engine {
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine(Engine&&) = default;
	virtual ~Engine() {} = default;

	void initialize(Game&) final;
	void shutdown() final;

	void tick(const Heartbeat&) final;

protected:
	virtual void onInitialize() = 0;
	virtual void onShutdown() = 0;
	virtual void onTick(const Heartbeat&) = 0;

	Game& game() final const;
	EventBus& events() final const;

private:
	Game* gamePtr;
};

} /* namespace WheelsOfWar */
