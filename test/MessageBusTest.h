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
	CPPUNIT_TEST( testListen );
	CPPUNIT_TEST( testUnlisten );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {

	}

	void tearDown() {

	}

	void testListen() {
		WheelsOfWar::MessageBus msgbus;

		msgbus.listen<TestMessage>([&](auto arg) {

		});

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 1);
	}

	void testUnlisten() {
		WheelsOfWar::MessageBus msgbus;
		auto listener = [](const TestMessage& msg){

		};

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 0);

		msgbus.unlisten<TestMessage>(listener);

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 0);

		msgbus.listen<TestMessage>(listener);

		CPPUNIT_ASSERT(msgbus.listenerCount<TestMessage>() == 1);

		msgbus.unlisten<TestMessage>(listener);

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
