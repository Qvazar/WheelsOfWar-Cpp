#pragma once

#include <forward_list>
#include <memory>
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

	virtual void initialize() = 0;
	virtual void shutdown() = 0;

	virtual void update(const Heartbeat&) = 0;
	virtual void tick(const Heartbeat&) = 0;

	void setScene(const ScenePtr& scene) final {
		auto oldScene = this->scene;
		this->scene = scene;
		this->onSetScene(oldScene, scene);
	}

protected:
	virtual void onSetScene(const ScenePtr& oldScene, const ScenePtr& newScene) {};

private:
	ScenePtr scene;
};

} /* namespace WheelsOfWar */
