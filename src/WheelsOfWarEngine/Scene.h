#pragma once

#include <memory>
#include "Entity.h"
#include "Heartbeat.h"

namespace WheelsOfWarEngine {

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

	void setActive(bool active) noexcept;
	bool isActive() const;

public:

	class SceneEntityMap : public Entity::EntityMap {
		virtual void onSet(const string& id, Entity* partPtr) override {
			Entity::EntityMap::onSet(id, partPtr);
			if (partPtr) {
				partPtr->scene = this;
			}
		}
	};

	SceneEntityMap entities;
private:
	bool active;
};

using ScenePtr = std::shared_ptr<Scene>;

}
