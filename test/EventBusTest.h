#pragma once

#ifdef DEBUG

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/WheelsOfWarEngine/EventBus.h"

namespace WheelsOfWarTest {

class EventBusTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( EventBusTest );
	CPPUNIT_TEST( testOn );
	CPPUNIT_TEST( testOff );
	CPPUNIT_TEST( testSend );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {

	}

	void tearDown() {

	}

	void testOn() {
		WheelsOfWar::EventBus eventBus;

		eventBus.on<TestEvent>([&](auto arg) {

		});

		CPPUNIT_ASSERT(eventBus.count<TestEvent>() == 1);

		eventBus.on<TestEvent>([&](auto arg) {

		});

		CPPUNIT_ASSERT(eventBus.count<TestEvent>() == 2);
	}

	void testOff() {
		WheelsOfWar::EventBus eventBus;
		auto listener1 = [](const TestEvent& msg){

		};

		auto listener2 = [](const TestEvent& msg){

		};

		CPPUNIT_ASSERT(eventBus.count<TestEvent>() == 0);

		CPPUNIT_ASSERT_NO_THROW(eventBus.off(listener1));

		CPPUNIT_ASSERT(eventBus.count<TestEvent>() == 0);

		eventBus.on(listener1);
		eventBus.on(listener2);

		CPPUNIT_ASSERT(eventBus.count<TestEvent>() == 2);

		eventBus.off(listener1);

		CPPUNIT_ASSERT(eventBus.count<TestEvent>() == 1);

		eventBus.off(listener2);

		CPPUNIT_ASSERT(eventBus.count<TestEvent>() == 0);
	}

	void testSend() {
		WheelsOfWar::EventBus eventBus;

		bool firstCalled = false;
		bool secondCalled = false;

		auto listener1 = [&firstCalled](const TestEvent& msg){
			firstCalled = true;
		};

		auto listener2 = [&secondCalled](const TestEvent& msg){
			secondCalled = true;
		};

		CPPUNIT_ASSERT_NO_THROW(eventBus.send(testMsg));
		CPPUNIT_ASSERT(!firstCalled && !secondCalled);

		eventBus.on(listener1);
		CPPUNIT_ASSERT_NO_THROW(eventBus.send(testMsg));
		CPPUNIT_ASSERT(firstCalled && !secondCalled);
		firstCalled = false;

		eventBus.on(listener2);
		CPPUNIT_ASSERT_NO_THROW(eventBus.send(testMsg));
		CPPUNIT_ASSERT(firstCalled && secondCalled);
}

private:

	struct TestEvent {

	} testMsg;

};

}