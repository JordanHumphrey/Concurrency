#pragma once
#include <mutex>
// Locking one mutex at a time in a comparison operator

/*
	get_detail() retrieves the value while protecting
	it with a lock. This reduces the locking period
	and eliminates possibility of dead lock by only 
	holding lock when absolutely necessary. 
	This is, however, exposed to a minor race condition
	where the values were the same at one point, but 
	now could be different in between the independent 
	lock calls. It might be better then, to use a plain
	std::mutex.
*/
class Y
{
private:
	int some_detail;
	mutable std::mutex m;
	int get_detail() const
	{
		std::lock_guard<std::mutex> lock_a(m);
		return some_detail;
	}
public:
	Y(int sd) :some_detail(sd) {}
	friend bool operator==(Y const& lhs, Y const& rhs)
	{
		if (&lhs == &rhs)
			return;
		int const lhs_value = lhs.get_detail();
		int const rhs_value = rhs.get_detail();
		return lhs_value == rhs_value;
	}
};