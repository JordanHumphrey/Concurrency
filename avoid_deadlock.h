#pragma once
#include <mutex>

class some_big_object{};
void swap(some_big_object& lhs, some_big_object& rhs);

class X
{
private:
	some_big_object some_detail;
	std::mutex m;
public:
	X(some_big_object const& sd) : some_detail(sd) {}
	friend void swap(X& lhs, X& rhs)
	{
		if (&lhs == &rhs)
			return;
		/*std::lock(lhs.m, rhs.m);
		std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
		std::lock_guard<std::mutex> lock_a(rhs.m, std::adopt_lock);
		swap(lhs.some_detail, rhs.some_detail);*/

		// An easier way to do it
		std::scoped_lock<std::mutex, std::mutex> guard(lhs.m, rhs.m);
		swap(lhs.some_detail, rhs.some_detail);
	}
};

/*
	Arguements are checked to ensure different instances,
	since attempting acquiring a lock on mutex when u already hold it is 
	undefined behavior.

	Adopt_Lock parameter is supplied to indicate to the lock_guard
	objects that the mutexes are already locked, and should adopt ownership
	of the lock on the mutex rather than attempt to lock the mutex
	in the constructor. Ensures mutexes are correctly unlocked on
	function exit
*/