#pragma once

#include <condition_variable>
#include <mutex>
#include "EventBus.h"
#include "Game.h"
#include "Heartbeat.h"

namespace WheelsOfWarEngine {

class Engine {
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine(Engine&&) = default;
	virtual ~Engine() = default;

	void initialize(Game&);
	void shutdown();

	void update(const Heartbeat&);
	void tick(const Heartbeat&);

protected:
	virtual void onInitialize() = 0;
	virtual void onShutdown() = 0;
	virtual void onUpdate(const Heartbeat&) = 0;
	virtual void onTick(const Heartbeat&) = 0;

	Game& game() const;
	EventBus& events() const;

private:
	Game* gamePtr = nullptr;
	std::mutex mutex;
	std::condition_variable cv;
};

} /* namespace WheelsOfWar */
