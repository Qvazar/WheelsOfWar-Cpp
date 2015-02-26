#pragma once

#include <memory>
#include "Entity.h"
#include "Heartbeat.h"

namespace WheelsOfWar {

class Scene {
public:
	Scene() = default;
	Scene(const Scene&) = default;
	Scene(Scene&&) = default;
	~Scene() = default;

	Scene& operator=(const Scene&) = default;
	Scene& operator=(Scene&&) = default;

	void update(const Heartbeat&) noexcept;
	void applyUpdate() noexcept;
	void tick(const Heartbeat&) noexcept;

	void setActive(bool active);
	bool isActive();

public:

	class SceneEntityMap : public Entity::EntityMap {
		virtual void onSet(const string& id, P* partPtr) override {
			if (partPtr) {
				partPtr->scene = this;
			}
		}
	};

	SceneEntityMap entities;
private:
	bool active;
};

typedef std::shared_ptr<Scene> ScenePtr;

}