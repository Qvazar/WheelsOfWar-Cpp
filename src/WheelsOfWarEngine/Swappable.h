#pragma once

#include <memory>
#include <tuple>

namespace WheelsOfWarEngine {

using namespace std;

template<class DataT>
class Swappable {
public:
	Swappable()
	: tuple{make_tuple(make_unique<DataT>(), make_unique<DataT>())}
	{ }
	
	virtual ~Swappable();

	DataT& current() {
		return *(get<0>(tuple));
	}

	DataT& next() {
		return *(get<1>(tuple));
	}

	void swap() {
		tuple.swap();
	}

private:
	typedef unique_ptr<DataT> DataTPtr;
	tuple<DataTPtr, DataTPtr> tuple;
};

}