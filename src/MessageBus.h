/*
 * MessageBus.h
 *
 *  Created on: Jan 16, 2015
 *      Author: sj
 */

#ifndef SRC_MESSAGEBUS_H_
#define SRC_MESSAGEBUS_H_

#include <forward_list>
#include <functional>
#include <map>
#include <memory>
#include <typeinfo>

using namespace std;

namespace WheelsOfWar {

class MessageBus {
public:
	MessageBus();
	virtual ~MessageBus();

	template<typename T>
	void send(const T& message) const {
		auto msgbus = getTypedMessageBus<T>();
		msgbus->send(message);
	}

	template<typename T>
	void listen(const function<void(const T&)>& listener) {
		auto pTypedMsgbus = getTypedMessageBus<T>();
		if (pTypedMsgbus == nullptr) {
			pTypedMsgbus = &createTypedMessageBus<T>();
		}
		pTypedMsgbus->listen(listener);
	}

	template<typename T>
	void unlisten(const function<void(const T&)>& listener) {
		auto pTypedMsgbus = getTypedMessageBus<T>();
		if (pTypedMsgbus != nullptr) {
			pTypedMsgbus->unlisten(listener);
		}
	}

	template<typename T>
	unsigned int listenerCount() const {
		auto pTypedMsgbus = getTypedMessageBus<T>();
		if (pTypedMsgbus == nullptr) {
			return 0;
		} else {
			return distance(pTypedMsgbus->listeners.begin(), pTypedMsgbus->listeners.end());
		}
	}

private:
	class TypedMessageBusBase {
	public:
		TypedMessageBusBase(){}
		virtual ~TypedMessageBusBase(){}
	};

	template<typename T>
	class TypedMessageBus : public TypedMessageBusBase {
		friend class MessageBus;
	public:
		void send(const T& msg) {
			for (auto l : this->listeners) {
				l(msg);
			}
		}

		void listen(const function<void(const T&)>& listener) {
			this->listeners.push_front(listener);
		}

		void unlisten(const function<void(const T&)>& listener) {
			this->listeners.remove(listener);
		}

	private:
		forward_list<function<void(const T&)> > listeners;
	};

	template<typename T>
	TypedMessageBus<T>* getTypedMessageBus() const {
		const type_info& key = typeid(T);

		auto iter = typedMsgbusMap.find(key);

		if (iter == typedMsgbusMap.end()) {
			return nullptr;
		} else {
			return static_cast<TypedMessageBus<T>*>(iter->second.get());
		}
	}

	template<typename T>
	TypedMessageBus<T>& createTypedMessageBus() {
		const type_info& key = typeid(T);
		auto ptr = &typedMsgbusMap[key];

		if ((*ptr) == nullptr) {
			ptr->reset(new TypedMessageBus<T>);
		}

		return *static_cast<TypedMessageBus<T>*>(ptr->get());
	}

	map<type_info, unique_ptr<TypedMessageBusBase>> typedMsgbusMap;
};

} /* namespace WheelsOfWar */

#endif /* SRC_MESSAGEBUS_H_ */
