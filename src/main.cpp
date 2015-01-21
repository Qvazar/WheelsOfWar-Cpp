/*
 * main.cpp
 *
 *  Created on: Jan 20, 2015
 *      Author: sj
 */

#include <algorithm>
#include <string>

#ifdef DEBUG
#include "../test/TestRunner.h"
#endif

int main(int argc, char* argv[]) {
#ifdef DEBUG
	if (std::find(argv, argv+argc, "-test") != argv+argc) {
		WheelsOfWarTest::runTests();
	}
#endif
}


