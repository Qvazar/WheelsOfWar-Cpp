#pragma once

#include <algorithm>
#include <deque>
#include <iterator>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <typeinfo>
#include "thread-util.h"

using namespace std;

namespace WheelsOfWarEngine {

	class EventBus {
		typedef std::unordered_multimap<type_index, std::weak_ptr<HandlerBase>> HandlersMap;
		friend class HandlerBase;

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

		template<class E>
		using HandlerPtr<E> = std::shared_ptr<Handler<E>>;

	private:
		template<class E, class H>
		class TypedHandler : public HandlerBase<E> {
		public:
			Handler(EventBus& eventBus, H handler)
			: HandlerBase<E>(eventBus), actualHandler{handler} {}

			Handler(const Handler<E, H>&) = default;
			Handler(Handler&&) = default;

			Handler<E, H>& operator=(const Handler<E, H>&) = default;
			Handler<E, H>& operator=(Handler<E, H>&&) = default;

			virtual void operator()(const E& e) const override {
				this->actualHandler(e);
			}
		private:
			H actualHandler;
		};

	public:
		EventBus() : handlersPtr{make_shared<HandlersMap>()} {};

		EventBus(const EventBus& other) {
			*this = other;
		}

		EventBus(EventBus&& other) {
			*this = forward(other);
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
		static HandlerPtr<E> createHandler(const H& plainHandler) noexcept {
			return std::make_shared<TypedHandler<E, H>>(plainHandler);
		}

		template<class E>
		void send(const E& event) noexcept const {
			auto eType = type_index(typeid(E));
			auto& handlers = *(this->handlersPtr);

			auto range = handlers.equal_range(eType);

			std::for_each(range.first, range.second, [&](const auto& hPtr){
				auto handlerPtr = static_cast<HandlerPtr<E>>(hPtr.lock());
				if (handlerPtr) {
					(*handlerPtr)(event);
				}
			});
		}

		template<class E>
		void on(const HandlerPtr<E>& handlerPtr) noexcept {
			auto eType = type_index(typeid(E));

			lock_guard<mutex> lk(handlersMutex);

			auto handlersPtr = make_shared<HandlersMap>(*(this->handlersPtr));
			handlersPtr->emplace(eType, handlerPtr);

			this->handlersPtr = move(handlersPtr);
		}

		template<class E, class H>
		HandlerPtr<E> on(const H& plainHandler) noexcept {
			auto handlerPtr = this->createHandler<E>(plainHandler);
			this->on(handlerPtr);
			return handlerPtr;
		}

		template<class E>
		void off(const HandlerPtr<E>& handler) noexcept {
			this->off<E>(handler.get());
		}

		template<class E>
		unsigned int count() noexcept const {
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
				while ((it = std::find_if(it, range.second, [&](const auto& hPtr) { return handlerBasePtr == hPtr.lock().get(); })) != this->handlers.end()) {
					it = this->handlers.erase(it);
				}
			}

			this->handlersPtr = move(handlersPtr);
		}

		shared_ptr<HandlersMap> handlersPtr;
		mutex handlersMutex;
	};
}