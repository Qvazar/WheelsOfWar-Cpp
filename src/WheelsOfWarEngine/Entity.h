#pragma once

#include <algorithm>
#include <forward_list>
#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>
#include "Component.h"
#include "EventBus.h"
#include "Heartbeat.h"

namespace WheelsOfWarEngine {

class Entity {
public:
	Entity() : entities(this), components(this) {};
	Entity(const Entity&) = default;
	Entity(Entity&&) = default;
	virtual ~Entity() = default;

	Entity& operator=(const Entity&) = default;
	Entity& operator=(Entity&&) = default;

	void update(const Heartbeat&) noexcept;
	void applyUpdate() noexcept;
	void tick(const Heartbeat&) noexcept;

	Scene* getScene() const {
		Entity* ep = this;
		while (!ep->scene && ep->parent) {
			ep = ep->parent;
		}

		return ep->scene;
	}

	template<typename P>
	class PartMap {
		typedef std::unordered_map<std::string, std::unique_ptr<P>> _partMapType;
		
	public:
		PartMap(const Entity* entity = nullptr)
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

		virtual ~PartMap() = default;

		void set(const std::string& id, P* partPtr) noexcept {
			this->parts[id].reset(partPtr);
			this->onSet(id, partPtr);
		}

		void set(const std::initializer_list<std::pair<std::string, P*>>& parts) noexcept {
			for (const auto& p : parts) {
				this->set(p->first, p->second);
			}
		}

		void remove(const std::string& id) noexcept {
			this->remove(this->parts.find(id));
		}

		void remove(P* partPtr) noexcept {
			auto it = this->parts.begin();

			while ((it = std::find_if(
					it,
					this->parts.end(),
					[partPtr](const auto& p) {
						return p->second.get() == partPtr;
					})) != this->parts.end()) {
				this->remove(it);
				++it;
			}
		}

		void remove(const _partMapType::const_iterator& it) {
			if (it != this->parts.cend()) {
				auto id = it->first;
				P* part = it->second.get();

				this->parts.erase(it);
				this->onRemove(id, part);
			}
		}

		P* operator[](const std::string& id) const {
			auto it = this->parts.find(id);
			return (it == this->parts.end()) ? nullptr : it->second.get();
		}

		_partMapType::iterator begin() {
			return this->parts.begin();
		}

		_partMapType::const_iterator begin() const {
			return this->parts.begin();
		}

		_partMapType::const_iterator cbegin() const {
			return this->parts.cbegin();
		}

		_partMapType::iterator end() {
			return this->parts.end();
		}

		_partMapType::const_iterator end() const {
			return this->parts.end();
		}

		_partMapType::const_iterator cend() const {
			return this->parts.cend();
		}

	protected:
		virtual void onSet(const string& id, P* partPtr) {
			if (partPtr) {
				partPtr->parent = this->entity;
			}
		};

		virtual void onRemove(const string& id, P* partPtr) { };

		_partMapType parts;
		const Entity* entity;
	};

	typedef PartMap<Entity> EntityMap;
	typedef PartMap<Component> ComponentMap;

	EntityMap entities;
	ComponentMap components;
	EventBus events;
private:
	Entity* parent;
	Scene* scene;
};

} /* namespace WheelsOfWar */
