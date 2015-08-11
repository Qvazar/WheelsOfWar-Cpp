#pragma once

#include <mutex>
#include <stack>
#include "EventBus.h"
#include "Scene.h"

using namespace std;

namespace WheelsOfWarEngine {

class SceneManager {
	typedef stack<ScenePtr> SceneStack;

public:
	SceneManager(EventBus&);
	SceneManager(const SceneManager&) = default;
	SceneManager(SceneManager&&) = default;
	~SceneManager() = default;

	void set(ScenePtr& scene);
	void push(ScenePtr& scene);
	ScenePtr pop();
	ScenePtr popIf(const ScenePtr&);
	ScenePtr get() const;

private:
	EventBus* eventsPtr;
	unique_ptr<SceneStack> scenesPtr;
	mutex scenesMutex;
};

}