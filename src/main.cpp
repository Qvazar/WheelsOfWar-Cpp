/*
 * main.cpp
 *
 *  Created on: Jan 20, 2015
 *      Author: sj
 */

#include <algorithm>
#include <iostream>
#include <string>

#ifdef DEBUG
#include "../test/TestRunner.h"
#endif

int main(int argc, char* argv[]) {
	cout << "Hello world!" << endl;
#ifdef DEBUG
	//if (std::find(argv, argv+argc, "-test") != argv+argc) {
	if (std::string(argv[1]) == "-test") {
		cout << "Running tests..." << endl;
		WheelsOfWarTest::runTests();
		cout << "Done!" << endl;
	}
#endif
}


