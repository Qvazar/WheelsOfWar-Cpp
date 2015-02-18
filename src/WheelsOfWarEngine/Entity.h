#pragma once

#include <algorithm>
#include <forward_list>
#include <initializer_list>
#include <map>
#include <string>
#include "Component.h"
#include "EventBus.h"
#include "Heartbeat.h"

namespace WheelsOfWarEngine {

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
		friend class Entity;
		friend class ComponentMap;
	private:
		PartMap(const Entity* entity)
		: entity(entity);

		PartMap(const PartMap& other) noexcept {
			*this = other;
		}

		PartMap(PartMap&&) = default;

		PartMap& operator=(const PartMap&) noexcept {
			this->parts.clear();

			for (const auto& p : other.parts) {
				this->parts[p->first] = p->second->clone();
			}

			return *this;
		}

		PartMap& operator=(PartMap&&) = default;
	public:
		virtual ~PartMap() = default;

		void set(const std::string& id, P* partPtr) noexcept {
			this->parts[id].reset(partPtr);
			if (partPtr) {
				partPtr->parent = this->entity;
			}
		}

		void set(const std::initializer_list<std::pair<std::string, P*>>& parts) noexcept {
			for (const auto& p : parts) {
				this->set(p->first, p->second);
			}
		}

		void remove(const std::string& id) noexcept {
			this->parts.erase(id);
		}

		void remove(P* partPtr) noexcept {
			std::remove_if(this->parts.begin(), this->parts.end(), [partPtr](const auto& p) {
				return p->second.get() == partPtr;
			});
		}

		P& get(const std::string& id) const {
			return this->parts.at(id);
		}

	private:
		std::map<std::string, std::unique_ptr<P>> parts;
		Entity* entity;
	};

	typedef PartMap<Entity> EntityMap;

	class ComponentMap : PartMap<Component> {
	public:
		template<typename T>
		std::forward_list<T*> find<T>() const noexcept {
			std::forward_list<T*> matches;

			for (const auto& p : this->parts) {
				T* tPtr = dynamic_cast<T*>(p->second.get());
				if (tPtr) {
					matches.push_front(tPtr);
				}
			}

			return matches;
		}
	};

	EntityMap entities;
	ComponentMap components;
	EventBus events;
private:
	Entity* parent;
};

} /* namespace WheelsOfWar */
