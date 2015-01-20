/*
 * TestRunner.h
 *
 *  Created on: Jan 19, 2015
 *      Author: sj
 */

#ifndef TEST_TESTRUNNER_H_
#define TEST_TESTRUNNER_H_

#ifdef DEBUG
#include "MessageBusTest.h"

namespace WheelsOfWarTest {

class TestRunner {
private:
	TestRunner() {}
public:
	static void runTests() const;
};

}

#endif

#endif /* TEST_TESTRUNNER_H_ */
