/*
 * Component.h
 *
 *  Created on: Jan 16, 2015
 *      Author: sj
 */

#ifndef SRC_COMPONENT_H_
#define SRC_COMPONENT_H_

#include "UpdateEvent.h"

namespace WheelsOfWar {

class Component {
public:
	Component();
	virtual ~Component();

	virtual void update(const UpdateEvent&) = 0;
	virtual void swap(void) = 0;
};

} /* namespace WheelsOfWar */

#endif /* SRC_COMPONENT_H_ */
