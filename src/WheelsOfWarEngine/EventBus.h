#pragma once

#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include <typeinfo>
#include "thread-util.h"

namespace WheelsOfWarEngine {

	class EventBus {
	public:
		class HandlerBase {
		public:
			virtual ~HandlerBase() {} = 0;
		};

		template<class E>
		class Handler : public HandlerBase {
		protected:
			Handler() {}
		public:
			virtual void operator()(const E& e) const = 0;
		};

		template<class E>
		using HandlerPtr<E> = std::shared_ptr<Handler<E>>;

	private:
		template<class E, class H>
		class TypedHandler : public HandlerBase<E> {
		public:
			Handler(H handler) : actualHandler{handler} {}
			Handler(const Handler<E, H>&) = default;
			Handler(Handler&&) = default;
			virtual ~Handler() = default;

			Handler<E, H>& operator=(const Handler<E, H>&) = default;
			Handler<E, H>& operator=(Handler<E, H>&&) = default;

			virtual void operator()(const E& e) const override {
				this->actualHandler(e);
			}
		private:
			H actualHandler;
		};
	public:

		EventBus() = default;

		EventBus(const EventBus& other) {
			*this = other;
		}

		EventBus(EventBus&& other) {
			*this = other;
		}

		~EventBus() = default;

		EventBus& operator=(const EventBus& other) {
			auto thisLock = std::unique_lock{mutex};
			auto otherLock = std::shared_lock{other.mutex};
			std::lock(thisLock, otherLock);

			this->handlers = other.handlers;

			return *this;			
		};

		EventBus& operator=(EventBus&& other) {
			auto thisLock = std::unique_lock{mutex};
			auto otherLock = std::shared_lock{other.mutex};
			std::lock(thisLock, otherLock);

			this->handlers = std::move(other.handlers);

			return *this;
		}

		template<class E, class H>
		HandlerPtr<E> createHandler(const H& plainHandler) noexcept {
			return std::make_shared<TypedHandler<E, H>>(plainHandler);
		}

		template<class E>
		void send(const E& event) noexcept const {
			auto eType = type_index(typeid(E));

			lockShared(mutex, [&]() {
				auto range = this->handlers.equal_range(eType);

				if (range.first != this->handlers.end()) {
					std::for_each(range.first, range.second, [&](const auto& hPtr){
						*(static_cast<HandlerPtr<E>>(hPtr))(event);
					});
				}
			});
		}

		template<class E>
		void on(const HandlerPtr<E>& handlerPtr) noexcept {
			auto eType = type_index(typeid(E));

			lockUnique(mutex, [&](){
				this->handlers.emplace(eType, handlerPtr);
			});
		}

		template<class E, class H>
		HandlerPtr<E> on(const H& plainHandler) noexcept {
			auto handlerPtr = this->createHandler<E>(plainHandler);
			this->on(handlerPtr);
			return handlerPtr;
		}

		template<class E>
		void off(const HandlerPtr<E>& handler) noexcept {
			auto eType = type_index(typeid(E));

			lockUnique(mutex, [&](){
				auto range = this->handlers.equal_range(eType);

				if (range.first != this->handlers.end()) {
					auto it = range.first;
					while ((it = std::find_if(it, range.second, [&](const auto& hPtr) { return handler == hPtr; })) != this->handlers.end()) {
						it = this->handlers.erase(it);
					}
				}
			});
		}

		template<class E>
		unsigned int count() noexcept const {
			auto eType = type_index(typeid(E));
			return lockShared(mutex, [&]() {
				auto range = this->handlers.equal_range(eType);

				return std::distance(range.first, range.second);
			});
		}

	private:
		std::unordered_multimap<type_index, std::shared_ptr<HandlerBase>> handlers;
		mutable shared_timed_mutex mutex;
	};
}