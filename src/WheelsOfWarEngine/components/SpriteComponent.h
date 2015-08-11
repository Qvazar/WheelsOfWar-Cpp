#pragma once

#include "../Component.h"

namespace WheelsOfWarEngine {
	
	class SpriteComponent : public Component {
	public:
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(SpriteComponent&&) = default;
		~SpriteComponent() = default;

		SpriteComponent& operator=(const SpriteComponent&) = default;
		SpriteComponent& operator=(SpriteComponent&&) = default;
	};

}