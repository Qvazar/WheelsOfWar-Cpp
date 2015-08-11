#include <utility>
#include "SceneManager.h"
#include "thread-util.h"

using namespace std;

namespace WheelsOfWarEngine {

SceneManager::SceneManager(EventBus& eventBus)
: eventsPtr{&eventBus}, scenesPtr{make_unique<SceneStack>()} {

}

void SceneManager::set(ScenePtr& scene) {
	lock(scenesMutex, [&]() {
		auto newScenes = make_unique<SceneStack>();
		newScenes->push(scene);

		this->scenesPtr = move(newScenes);
	});
}

void SceneManager::push(ScenePtr& scene) {
	lock(scenesMutex, [&]() {
		auto newScenes = make_unique<SceneStack>(*this->scenesPtr);
		newScenes->push(scene);

		this->scenesPtr = move(newScenes);
	});
}

ScenePtr SceneManager::pop() {
	return lock(scenesMutex, [&]() {
		auto newScenes = make_unique<SceneStack>(*this->scenesPtr);
		ScenePtr popped = nullptr;

		if (!newScenes->empty()) {
			popped = newScenes->top();
			newScenes->pop();

			this->scenesPtr = move(newScenes);
		}

		return popped;
	});
}

ScenePtr SceneManager::popIf(const ScenePtr& scenePtr) {
	return lock(scenesMutex, [&]() {
		auto newScenes = make_unique<SceneStack>(*this->scenesPtr);
		ScenePtr popped = nullptr;

		if (!newScenes->empty()) {
			popped = newScenes->top();

			if (popped == scenePtr) {
				newScenes->pop();
				this->scenesPtr = move(newScenes);
			}
		}

		return popped;
	});	
}

ScenePtr SceneManager::get() const;
	return this->scenesPtr->top();
}