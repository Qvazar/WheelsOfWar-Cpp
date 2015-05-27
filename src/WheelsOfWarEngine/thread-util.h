#pragma once

#include <mutex>
#include <shared_mutex>
#include <utility>

using namespace std;

namespace WheelsOfWarEngine {

/*
template<class M = shared_timed_mutex>
class ReadWriteLock {
public:
	ReadWriteLock() {}
	explicit ReadWriteLock(const M& mutex) : mutex{mutex} {}
	explicit ReadWriteLock(M&& mutex) : mutex{forward(mutex)} {}
	ReadWriteLock(const ReadWriteLock&) = default;
	ReadWriteLock(ReadWriteLock&&) = default;
	~ReadWriteLock() = default;

	ReadWriteLock& operator=(const ReadWriteLock&) = default;
	ReadWriteLock& operator=(ReadWriteLock&&) = default;

	template<class Lock, class R, class F, class... Args>
	R lock(F fn, Args&&... args) {
		auto lock = this->getLock<Lock>();
		std::lock(lock);

		return fn(forward(args));
	}

	template<class R, class F, class... Args>
	R unique(F fn, Args&&... args) {
		return this->lock<unique_lock>(fn, forward(args));
	}

	template<class R, class F, class... Args>
	R shared(F fn, Args&&... args) {
		return this->lock<shared_lock>(fn, forward(args));
	}

	template<class Lock>
	Lock getLock() const {
		return Lock{mutex, std::defer_lock_t};
	}

	unique_lock getUniqueLock() const {
		return getLock<unique_lock>();
	}

	shared_lock getSharedLock() const {
		return getLock<shared_lock>();
	}

private:
	M mutex;
};
*/

template<class Lock, class M, class F, class R = void>
R lock(const M& mutex, F fn) {
	Lock lk{mutex};
	return fn();
}

template<class M, class F, class R = void>
R lock(const M& mutex, F fn) {
	return lock<lock_guard<M>, M, F, R>(mutex, fn);
}

template<class M, class F, class R = void>
R lockShared(const M& mutex, F fn) {
	return lock<shared_lock<M>, M, F, R>(mutex, fn);
}

}
