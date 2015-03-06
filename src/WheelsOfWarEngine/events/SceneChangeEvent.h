#pragma once

#include "../Scene.h"

namespace WheelsOfWarEngine {

struct SceneChangeEvent final {
	ScenePtr oldScene;
	ScenePtr newScene;
};

}