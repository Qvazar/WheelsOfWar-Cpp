/*
 * MessageBus.cpp
 *
 *  Created on: Jan 16, 2015
 *      Author: sj
 */

#include "MessageBus.h"

namespace WheelsOfWar {

MessageBus::MessageBus() {
	// TODO Auto-generated constructor stub

}

MessageBus::~MessageBus() {
	// TODO Auto-generated destructor stub
}

template<typename T>
void MessageBus::send(const T& message) const {
	auto listenerList = this->getListeners<T>();
	for (auto l : listenerList) {
		l(message);
	}
}

template<typename T>
void MessageBus::listen(const std::function<void(const T&)>& listener) {
	auto listenerList = this->getListeners<T>();

	if (listenerList == Listeners<T>::empty) {
		auto pListeners = new Listeners<T>;
		this->listenerMap[typeid(T)] = pListeners;
		listenerList = pListeners->list;
	}

	listenerList.push_front(listener);
}

template<typename T>
void MessageBus::unlisten(const std::function<void(const T&)>& listener) {
	auto listenerList = this->getListeners<T>();

	listenerList.remove(listener);
}

template<typename T>
MessageBus::ListenerList<T>& MessageBus::getListeners() {
	auto listenerIter = static_cast<Listeners<T> >(this->listenerMap.find(typeid(T)));

	if (listenerIter == this->listenerMap.end()) {
		return Listeners<T>::empty;
	} else {
		return listenerIter->second;
	}
}

} /* namespace WheelsOfWar */
