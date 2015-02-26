#pragma once

#include <algorithm>
#include <forward_list>
#include <iterator>
#include <unordered_map>
#include <memory>
#include <typeinfo>

namespace WheelsOfWarEngine {

	class EventBus {
	public:
		EventBus() = default;

		EventBus(const EventBus& other) noexcept {
			*this = other;
		}

		EventBus(EventBus&& other) = default;

		~EventBus() = default;

		EventBus& operator=(const EventBus& other) noexcept {
			this->typedBuses.clear();

			for (const auto& busPair : other.typedBuses) {
				this->typedBuses[busPair->first] = busPair->second->clone();
			}

			return *this;
		}

		EventBus& operator=(EventBus&& other) = default;

		template<typename E>
		void send(const E& event) noexcept const {
			auto eType = type_index(typeid(E));
			auto it = this->typedBuses.find(eType);

			if (it != this->typedBuses.end()) {
				const auto* bus = static_cast<TypedEventBus<E>*>(it->second.get());
				bus->send(event):
			}
		}

		template<typename E, typename H>
		void on(const H* handler) noexcept {
			auto& bl = this->typedBuses;
			auto eType = type_index(typeid(E));
			auto& busPtr = bl[eType];

			if (!busPtr) {
				busPtr.reset(new TypedEventBus<E>);
			}

			static_cast<TypedEventBus<E>*>(busPtr.get())->on(handler);
		}

		template<typename E, typename H>
		void off(const H* handler) noexcept {
			auto eType = type_index(typeid(E));
			auto it = this->typedBuses.find(eType);

			if (it == this->typedBuses.end()) {
				return;
			}

			auto& busPtr = it->second;

			static_cast<TypedEventBus<E>*>(busPtr.get())->off(handler);

			if (busPtr->handlers.empty()) {
				this->typedBuses.erase(it);
			}
		}

		template<typename E>
		unsigned int count() noexcept const {
			auto eType = type_index(typeid(E));
			auto it = this->typedBuses.find(eType);

			if (it == this->typedBuses.end()) {
				return 0;
			} else {
				return it->second->count();
			}
		}

	private:
		class TypedEventBusBase {
			virtual TypedEventBusBase* clone() const = 0;
		};

		template<typename E>
		class TypedEventBus : public TypedEventBusBase {
		public:
			TypedEventBus() = default;

			TypedEventBus(const TypedEventBus& other) {
				*this = other;
			}

			TypedEventBus(TypedEventBus&& other)
			: handlers(std::move(other.handlers)) {

			}

			~TypedEventBus() = default;

			TypedEventBus<E>& operator=(const TypedEventBus<E>& other) noexcept {
				this->handlers.clear();
				
				for (const auto& handlerPtr : other.handlers) {
					this->handlers.emplace_front(handlerPtr->clone());
				}

				return *this;
			}

			TypedEventBus<E>& operator=(TypedEventBus<E>&& other) noexcept {
				this->handlers = std::move(other.handlers);
				return *this;
			}

			virtual TypedEventBus<E>* clone() const override {
				return new TypedEventBus<E>(*this);
			}

			void send(const E& event) noexcept const {
				auto hl = this->handlers;
				for (const auto& h : hl) {
					(*h)(event);
				}
			}

			template<typename H>
			void on(const H* handlerPtr) noexcept {
				this->handlers.emplace_front(new Handler<H>(handlerPtr));
			}

			template<typename H>
			void off(const H* handlerPtr) noexcept {
				this->handlers.remove_if([handlerPtr](const auto& hPtr) {
					Handler<H>* handlerStructP = dynamic_cast<Handler<H>*>(hPtr.get());
					return handlerStructP != nullptr && handlerStructP->handlerPtr == handlerPtr;
				});
			}

			unsigned int count() noexcept const {
				return std::distance(this->handlers.begin(), this->handlers.end());
			}

		private:
			struct HandlerBase {
				virtual void operator()(const E&) const = 0;
				virtual HandlerBase* clone() const = 0;
			};

			template<typename H>
			struct Handler : public HandlerBase {
				H* handlerPtr;

				virtual void operator()(const E& e) const override {
					(*this->handlerPtr)(e);
				}

				virtual Handler<H>* clone() const override {
					return new Handler<H> { this->handlerPtr };
				}
			};

			std::forward_list<unique_ptr<HandlerBase>> handlers;
		};

		std::unordered_map<type_index, std::unique_ptr<TypedEventBusBase>> typedBuses;
	};
}