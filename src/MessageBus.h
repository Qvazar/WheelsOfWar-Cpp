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
	void send(const T& message) const;

	template<typename T>
	void listen(const std::function<void(const T&)>&);

	template<typename T>
	void unlisten(const std::function<void(const T&)>&);

	template<typename T>
	using ListenerList = std::forward_list<std::function<void(const T&)>& >;

private:
	struct ListenersBase {
	};

	template<typename T>
	struct Listeners : public ListenersBase {
		ListenerList<T> list;
		static const ListenerList<T> empty;
		//static const std::type_info type_info = typeid(T);
	};

	template<typename T>
	ListenerList<T>& getListeners();

	std::map<std::type_info, const ListenersBase*> listenerMap;
};

} /* namespace WheelsOfWar */

#endif /* SRC_MESSAGEBUS_H_ */
