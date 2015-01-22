/*
 * MessageBusTest.h
 *
 *  Created on: Jan 19, 2015
 *      Author: sj
 */

#ifndef TEST_MESSAGEBUSTEST_H_
#define TEST_MESSAGEBUSTEST_H_

#ifdef DEBUG

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/MessageBus.h"

namespace WheelsOfWarTest {

class MessageBusTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( MessageBusTest );
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
		WheelsOfWar::MessageBus msgbus;

		msgbus.on<TestMessage>([&](auto arg) {

		});

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 1);

		msgbus.on<TestMessage>([&](auto arg) {

		});

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 2);
	}

	void testOff() {
		WheelsOfWar::MessageBus msgbus;
		WheelsOfWar::MessageBus::Listener<TestMessage> listener1 = [](const TestMessage& msg){

		};

		WheelsOfWar::MessageBus::Listener<TestMessage> listener2 = [](const TestMessage& msg){

		};

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 0);

		CPPUNIT_ASSERT_NO_THROW(msgbus.off(listener1));

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 0);

		msgbus.on(listener1);
		msgbus.on(listener2);

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 2);

		msgbus.off(listener1);

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 1);

		msgbus.off(listener2);

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 0);
	}

	void testSend() {
		WheelsOfWar::MessageBus msgbus;

		bool firstCalled = false;
		bool secondCalled = false;

		WheelsOfWar::MessageBus::Listener<TestMessage> listener1 = [&firstCalled](const TestMessage& msg){
			firstCalled = true;
		};

		WheelsOfWar::MessageBus::Listener<TestMessage> listener2 = [&secondCalled](const TestMessage& msg){
			secondCalled = true;
		};

		CPPUNIT_ASSERT_NO_THROW(msgbus.send(testMsg));

		msgbus.on(listener1);
		msgbus.on(listener2);

		msgbus.send(testMsg);

		CPPUNIT_ASSERT(firstCalled && secondCalled);
}

private:

	struct TestMessage {

	} testMsg;

};

CPPUNIT_TEST_SUITE_REGISTRATION( MessageBusTest );

}

#endif /* DEBUG */

#endif /* TEST_MESSAGEBUSTEST_H_ */
