#pragma once

namespace WheelsOfWarEngine {

template<typename T, class CollectionT, typename ExtractorT>
struct TypeCheckingIterator : std::iterator<std::forward_iterator_tag, T> {
public:
	TypeCheckingIterator(const CollectionT& collection)
	: current(collection.begin()), end(collection.end()) {
		this->operator++();
	}

	TypeCheckingIterator(const TypeCheckingIterator&) = default;

	TypeCheckingIterator(TypeCheckingIterator&&) = default;

	~TypeCheckingIterator() = default;

	TypeCheckingIterator<T>& operator++() { // prefix
		this->current = std::find_if(this->current, this->end, [](const auto& v) {
				return dynamic_cast<T*>(*(this->extractValue(v))) != nullptr;
			});

		return *this;
	}

	TypeCheckingIterator<T> operator++(int) { // postfix
		TypeCheckingIterator<T, IterT, ExtractorT> tmp(*this);

		this->operator++();

		return tmp;
	}

	T& operator*() {
		return this->extractValue(*current);
	}

	const T& operator*() const {
		return this->extractValue(*current);
	}

	T* operator->() {
		return &(this->extractValue(*current));
	}

	const T* operator->() const {
		return &(this->extractValue(*current));
	}

	bool operator==(const TypeCheckingIterator<T, IterT, ExtractorT>& other) {
		return *this == *other;
	}

protected:
	T extractValue(const CollectionT::value_type val) {
		return extractor(val);
	}

private:
	IterT current;
	IterT end;
	ExtractorT extractor;
};

template<typename T, class CollectionT>
struct TypeCheckingIterator : std::iterator<std::forward_iterator_tag, T> {
};
}