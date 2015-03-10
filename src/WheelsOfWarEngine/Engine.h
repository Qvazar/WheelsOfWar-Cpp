#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
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
	virtual void onUpdate(const Heartbeat&) = 0;
	virtual void onTick(const Heartbeat&) = 0;

	Game& game() final const;
	EventBus& events() final const;

private:
	void run();

	Game* gamePtr;
	atomic<bool> running = false;
	queue<Heartbeat> heartbeats;
	mutex heartbeatsMutex;
	condition_variable heartbeatsCv;
	thread runThread;
};

} /* namespace WheelsOfWar */
