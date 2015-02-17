#pragma once

#include <forward_list>
#include <initializer_list>
#include <map>
#include <string>
#include "Component.h"
#include "Heartbeat.h"

namespace WheelsOfWarEngine {

template<typename... ComponentTypes>
class Entity {
public:
	Entity() = default;
	Entity(const Entity&) = default;
	Entity(Entity&&) = default;
	virtual ~Entity() = default;

	Entity& operator=(const Entity&) = default;
	Entity& operator=(Entity&&) = default;

	void update(const Heartbeat&) noexcept;
	void applyUpdate() noexcept;
	void tick(const Heartbeat&) noexcept;

	template<typename P>
	class PartMap {
		friend class ComponentMap;
	private:
		PartMap() = default;
		PartMap(const PartMap&) = delete;
		PartMap(PartMap&&) = delete;
	public:
		virtual ~PartMap() = default;
		PartMap& operator=(const PartMap&) = delete;
		PartMap& operator=(PartMap&&) = delete;

		void add(const std::string&, P*) noexcept;
		void add(const std::initializer_list<std::pair<std::string, P*>>&) noexcept;
		void remove(const std::string&) noexcept;
		void remove(P*) noexcept;
		P& get(const std::string&) const;

	private:
		std::map<std::string, std::unique_ptr<P>> parts;
	};

	typedef PartMap<Entity> EntityMap;
	class ComponentMap : PartMap<Component> {
	public:
		template<typename T>
		std::forward_list<T*> find<T>() const noexcept;
	};

	EntityMap entities;
	ComponentMap components;
private:
	Entity* parent;
};

} /* namespace WheelsOfWar */

#include "Entity.tcc"