/*
 * MessageBusTest.h
 *
 *  Created on: Jan 19, 2015
 *      Author: sj
 */

#ifndef TEST_MESSAGEBUSTEST_H_
#define TEST_MESSAGEBUSTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/MessageBus.h"

namespace WheelsOfWarTest {

class MessageBusTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( MessageBusTest );
	CPPUNIT_TEST( testListen );
	CPPUNIT_TEST_SUITE_END();

public:
	void testListen() const {
		bool ret = false;

		WheelsOfWar::MessageBus msgbus;
		msgbus.listen<TestMessage>([&](auto arg) {
			ret = arg.isOk;
		});

		msgbus.send(testMsg);

		CPPUNIT_ASSERT(ret);
	}

private:

	struct TestMessage {
		bool isOk = true;
	} testMsg;

};

CPPUNIT_TEST_SUITE_REGISTRATION( MessageBusTest );

}

#endif /* TEST_MESSAGEBUSTEST_H_ */
