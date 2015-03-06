#pragma once

#include "EventBus.h"

namespace WheelsOfWarEngine {

template<class EventHolder, class E>
void sendEvent(const EventHolder& eventBus, const E& event)
	eventBus.send(event);
}

template<class E, class H>
EventBus::HandlerPtr<E> createEventHandler(const H& handler) {
	return EventBus::createHandler(handler);
}

template<class E>
void onEvent(EventHolder& eventBus, const EventBus::HandlerPtr<E>& handlerPtr) noexcept {
	
}
