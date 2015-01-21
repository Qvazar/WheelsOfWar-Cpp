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
	}

	void testOff() {
		WheelsOfWar::MessageBus msgbus;
		WheelsOfWar::MessageBus::Listener<TestMessage> listener = [](const TestMessage& msg){

		};

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 0);

		msgbus.off<TestMessage>(listener);

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 0);

		msgbus.on<TestMessage>(listener);

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 1);

		msgbus.off<TestMessage>(listener);

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 0);
}

private:

	struct TestMessage {
		bool isOk = true;
	} testMsg;

};

CPPUNIT_TEST_SUITE_REGISTRATION( MessageBusTest );

}

#endif /* DEBUG */

#endif /* TEST_MESSAGEBUSTEST_H_ */
