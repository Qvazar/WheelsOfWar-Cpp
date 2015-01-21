/*
 * TestRunner.h
 *
 *  Created on: Jan 19, 2015
 *      Author: sj
 */

#ifndef TEST_TESTRUNNER_H_
#define TEST_TESTRUNNER_H_

#ifdef DEBUG
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "MessageBusTest.h"

namespace WheelsOfWarTest {
	void runTests() {
		CppUnit::TextUi::TestRunner runner;
		CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
		runner.addTest( registry.makeTest() );
		runner.run();
	}
}

#endif

#endif /* TEST_TESTRUNNER_H_ */
