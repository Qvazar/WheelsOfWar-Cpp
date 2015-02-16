#pragma once

#include <algorithm>
#include <forward_list>
#include <map>
#include <typeinfo>

namespace WheelsOfWarEngine {

	class EventBus {
	public:
		EventBus() noexcept;
		EventBus(const EventBus&) noexcept;
		EventBus(EventBus&&) noexcept;
		~EventBus() noexcept;

		EventBus& operator=(const EventBus&) noexcept;
		EventBus& operator=(EventBus&&) noexcept;

		template<typename E>
		void send(const E& event) noexcept {
			auto eType = type_index(typeid(event));
			auto it = this->typedBuses.find(eType);

			if (it != this->typedBuses.end()) {
				auto bus = static_cast<TypedEventBus<E>*>(*(it->second));
				bus.send(event):
			}
		}

		template<typename E, typename H>
		void on(const H* handler) noexcept {
			auto eType = type_index(typeid(event));

			auto busPtr = this->typedBuses[eType];

			if (!busPtr) {
				busPtr.reset(new TypedEventBus<E>);
			}

			static_cast<TypedEventBus<E>*>(busPtr.get())->on(handler);
		}

		template<typename E, typename H>
		void off(const H* handler) noexcept {
			auto eType = type_index(typeid(event));
			auto it = this->typedBuses.find(eType);

			if (it == this->typedBuses.end()) {
				return;
			}

			auto busPtr = it->second;

			static_cast<TypedEventBus<E>*>(busPtr.get())->off(handler);

			if (busPtr->handlers.empty()) {
				this->typedBuses.erase(it);
			}
		}			

	private:
		class TypedEventBusBase {};

		template<typename E>
		class TypedEventBus : TypedEventBusBase {
			friend class EventBus;
		public:
			TypedEventBus() noexcept;
			TypedEventBus(const TypedEventBus&) = delete;
			TypedEventBus(TypedEventBus&&) = delete;
			~TypedEventBus() noexcept;

			void send(const E& event) noexcept {
				auto hl = this->handlers;
				for (auto& h : hl) {
					h(event);
				}
			}

			template<typename H>
			void on(const H* handler) noexcept {
				this->handlers.emplace_front([handler](const E& event){
					(*handler)(event);
				});

				this->handlerPointers.push_front(handler);
			}

			template<typename H>
			void off(const H* handler) noexcept {
				auto it = std::find(this->handlerPointers.begin(), this->handlerPointers.end(), handler);
				if (it != this->handlerPointers.end()) {
					auto idx = (it - this->handlerPointers.begin());
					this->handlers.erase_after(std::next(this->handlers.before_begin(), idx));
					this->handlerPointers.erase_after(std::next(this->handlerPointers.before_begin(), idx));
				}
			}

		private:
			std::forward_list<std::function<void(const E&)>> handlers;
			std::forward_list<void*> handlerPointers;
		};

		std::map<type_index, std::unique_ptr<TypedEventBusBase>> typedBuses;
	};
}