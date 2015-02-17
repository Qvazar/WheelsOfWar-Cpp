#pragma once

#include <map>

namespace WheelsOfWarEngine {
	template<typename K, V>
	std::map<K, V> cloneMap(const std::map<K, V>& source) {
		std::map<K, V> target;

		for (const auto& v : source) {
			target[v->first] = v->second->clone();
		}

		return target;
	}
}