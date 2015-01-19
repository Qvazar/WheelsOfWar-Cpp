#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "TestRunner.h"

void WheelsOfWarTest::TestRunner::runTests() const {
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest( registry.makeTest() );
	runner.run();
}

