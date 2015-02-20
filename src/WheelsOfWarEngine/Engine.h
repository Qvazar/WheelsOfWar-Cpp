#pragma once

#include "EventBus.h"
#include "Heartbeat.h"
#include "Scene.h"

namespace WheelsOfWar {

class Engine {
public:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine(Engine&&) = default;
	virtual ~Engine() {} = default;

	virtual void run();
	virtual void stop();

	virtual void update(const Heartbeat&) = 0;
	virtual void tick(const Heartbeat&) = 0;

	// Clears the scene-stack
	virtual void setScene(Scene*);

	virtual void pushScene(Scene*);

	virtual Scene* popScene();

private:
	EventBus events;
	bool isRunning = false;


};

} /* namespace WheelsOfWar */
