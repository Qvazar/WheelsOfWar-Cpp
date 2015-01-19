/*
 * MessageBus.h
 *
 *  Created on: Jan 16, 2015
 *      Author: sj
 */

#ifndef SRC_MESSAGEBUS_H_
#define SRC_MESSAGEBUS_H_

#include <map>
#include <typeinfo>
#include <functional>
#include <forward_list>

namespace WheelsOfWar {

class MessageBus {
public:
	MessageBus();
	virtual ~MessageBus();

	template<typename T>
	void send(const T& message) const {
		TypedMessageBusBase* typedMsgbus = this->typedMsgbusMap[typeid(T)];
		if (typedMsgbus != nullptr) {
			(static_cast< TypedMessageBus<T> >(typedMsgbus))->send(message);
		}
	}

	template<typename T>
	void listen(const std::function<void(const T&)>& listener) {
		TypedMessageBusBase* typedMsgbus = this->typedMsgbusMap[typeid(T)];
		if (typedMsgbus == nullptr) {
			this->typedMsgbusMap[typeid(T)] = typedMsgbus = new T;
		}

		(static_cast< TypedMessageBus<T> >(typedMsgbus))->listen(listener);
	}

	template<typename T>
	void unlisten(const std::function<void(const T&)>& listener) {
		TypedMessageBusBase* typedMsgbus = this->typedMsgbusMap[typeid(T)];
		if (typedMsgbus != nullptr) {
			(static_cast< TypedMessageBus<T> >(typedMsgbus))->unlisten(listener);
		}
	}

private:
	class TypedMessageBusBase {};

	template<typename T>
	class TypedMessageBus : public TypedMessageBusBase {
	public:
		void send(const T& msg) {
			for (auto l : this->listeners) {
				l(msg);
			}
		}

		void listen(const std::function<void(const T&)>& listener) {
			this->listeners.push_front(listener);
		}

		void unlisten(const std::function<void(const T&)>& listener) {
			this->listeners.remove(listener);
		}

	private:
		std::forward_list<std::function<void(const T&)>& > listeners;
	};

	std::map<std::type_info, const TypedMessageBusBase*> typedMsgbusMap;
};

} /* namespace WheelsOfWar */

#endif /* SRC_MESSAGEBUS_H_ */
