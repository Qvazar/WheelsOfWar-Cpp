#pragma once

#include <algorithm>
#include <deque>
#include <iterator>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include "thread-util.h"

namespace WheelsOfWarEngine {

using namespace std;

class EventBus {

public:
	class HandlerBase {
	protected:
		HandlerBase() {}
	public:
		virtual ~HandlerBase() = default;
	private:

	};

	template<class E>
	class Handler : public HandlerBase {
	protected:
		Handler(EventBus& parent) : parent(&parent) {}
	public:
		virtual void operator()(const E& e) const = 0;
		~Handler() {
			parent->off<E>(this);
		}
	private:
		EventBus* parent;
	};

	//template<typename E>

private:
	template<class E, class H>
	class TypedHandler : public Handler<E> {
	public:
		TypedHandler(EventBus& eventBus, H handler)
		: Handler<E>(eventBus), actualHandler{handler} {}

		TypedHandler(const TypedHandler<E, H>&) = default;
		TypedHandler(TypedHandler&&) = default;

		TypedHandler<E, H>& operator=(const TypedHandler<E, H>&) = default;
		TypedHandler<E, H>& operator=(TypedHandler<E, H>&&) = default;

		virtual void operator()(const E& e) const override {
			this->actualHandler(e);
		}
	private:
		H actualHandler;
	};

	typedef std::unordered_multimap<type_index, weak_ptr<HandlerBase>> HandlersMap;

public:

	EventBus() : handlersPtr{make_shared<HandlersMap>()} {};

	EventBus(const EventBus& other) {
		*this = other;
	}

	EventBus(EventBus&& other) {
		*this = forward<EventBus>(other);
	}

	~EventBus() = default;

	EventBus& operator=(const EventBus& other) {
		lock_guard<mutex> lk(handlersMutex);
		this->handlersPtr = make_shared<HandlersMap>(*(other.handlersPtr));
		return *this;
	};

	EventBus& operator=(EventBus&& other) {
		lock_guard<mutex> lk(handlersMutex);
		this->handlersPtr = move(other.handlersPtr);
		return *this;
	}

	template<class E, class H>
	static shared_ptr<Handler<E>> createHandler(const H& plainHandler) noexcept {
		return std::make_shared<TypedHandler<E, H>>(plainHandler);
	}

	template<class E>
	void send(const E& event) const {
		auto eType = type_index(typeid(E));
		auto& handlers = *(this->handlersPtr);

		auto range = handlers.equal_range(eType);

		std::for_each(range.first, range.second, [&](const weak_ptr<HandlerBase>& hPtr){
			auto handlerPtr = static_cast<shared_ptr<Handler<E>>>(hPtr.lock());
			if (handlerPtr) {
				(*handlerPtr)(event);
			}
		});
	}

	template<class E>
	void on(const shared_ptr<Handler<E>>& handlerPtr) noexcept {
		auto eType = type_index(typeid(E));

		lock_guard<mutex> lk(handlersMutex);

		auto handlersPtr = make_shared<HandlersMap>(*(this->handlersPtr));
		handlersPtr->emplace(eType, weak_ptr<HandlerBase>(handlerPtr));

		this->handlersPtr = move(handlersPtr);
	}

	template<class E, class H>
	shared_ptr<Handler<E>> on(const H& plainHandler) noexcept {
		auto handlerPtr = this->createHandler<E>(plainHandler);
		this->on(handlerPtr);
		return handlerPtr;
	}

	template<class E>
	void off(const shared_ptr<Handler<E>>& handler) noexcept {
		this->off<E>(handler.get());
	}

	template<class E>
	unsigned int count() const {
		auto eType = type_index(typeid(E));
		auto& handlers = *(this->handlersPtr);
		auto range = handlers.equal_range(eType);

		return std::distance(range.first, range.second);
	}

private:
	template<class E>
	void off(HandlerBase* handlerBasePtr) noexcept {
		auto eType = type_index(typeid(E));

		lock_guard<mutex> lk(handlersMutex);

		auto handlersPtr = make_shared<HandlersMap>(*(this->handlersPtr));

		auto range = handlersPtr->equal_range(eType);

		if (range.first != handlersPtr->end()) {
			auto it = range.first;
			while ((it = std::find_if(it, range.second, [&](const auto& hPtr) { return handlerBasePtr == hPtr.lock().get(); })) != this->handlersPtr->end()) {
				it = this->handlersPtr->erase(it);
			}
		}

		this->handlersPtr = move(handlersPtr);
	}

	shared_ptr<HandlersMap> handlersPtr;
	mutex handlersMutex;
};

}
