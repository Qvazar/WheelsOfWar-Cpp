#pragma once;

#include <string>

namespace WheelsOfWarEngine {

template<class R>
class Factory {
	virtual ~Factory() = default;
	virtual R create(const std::string&) = 0;
};

}