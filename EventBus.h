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

		}

		template<typename E, typename H>
		void on(const H* handler);

		template<typename E, typename H>
		void off(const H* handler);

	private:
		class TypedEventBusBase {};

		template<typename E>
		class TypedEventBus : TypedEventBusBase {
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