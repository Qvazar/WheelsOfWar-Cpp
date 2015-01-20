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
	for (auto p : this->typedMsgbusMap) {
		auto pTypedMsgbus = p.second;
		if (pTypedMsgbus) {
			delete pTypedMsgbus;
		}
	}
}

} /* namespace WheelsOfWar */
