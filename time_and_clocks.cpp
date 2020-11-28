#include <future>
#include <thread>
#include <mutex>
#include <list>
/*
* Durations
	In example 1, you can create a future that asynchoronusly
	executes a call to some_task, and when the value is needed,
	will wait a maximum of 35 milliseconds before moving on
	without acting on the data returned from the future.
	In this case, we are waiting for a future, so the function
	wait_for() returns a std::future_status::timeout if the wait
	times out, or ::ready if the future is ready, or ::deferred
	if the futures task is deferred.
*/

int some_task();
void do_something_with(int);
void example1()
{
	std::future<int> f = std::async(some_task);
	if (f.wait_for(std::chrono::milliseconds(35)) == std::future_status::ready)
		do_something_with(f.get());
}

/* Timepoints
	std::chrono::time_point<>, Arg1: clock to refer to, Arg2: unit of measurement.
	Example:
*/  std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes> time;
/*  This would hold time relative to system clock but in minutes.
	You can also add and subtract durations from instances of std::chrono::time_point
*/  auto time =
		std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(500);
/*  This is good for calculating an absolute timeout when you know how long a
*	block of code should take.
	You can also subtract one time point from another if they share the same clock:
*/	auto start = std::chrono::high_resolution_clock::now();
	auto stop = std::chrono::high_resolution_clock::now();
	auto res = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
/*
	Timepoints are used with the _until variants of the wait functions.
	If you have a maximum of 500 ms to wait for an event associated 
	with a condition variable, try the below code.
	This is the recommended way to wait for condition variables with
	a time limit if your not passing a predicate to wait.
	Condition variables requres unique_locks.
*/  std::condition_variable cv;
	bool done;
	std::mutex m;
	bool wait_loop()
	{
		auto const timeout = std::chrono::steady_clock::now() +
			std::chrono::milliseconds(500);
		std::unique_lock<std::mutex> lk(m);
		while (!done)
		{
			if (cv.wait_until(lk, timeout) == std::cv_status::timeout)
				break;
		}
		return done;
	}

	// Example of Quicksort vs Parallel Quicksort using Futures
	template<typename T>
	std::list<T>  sequential_quicksort(std::list<T> input)
	{
		if (input.empty())
		{
			return input;
		}
		std::list<T> result;
		result.splice(begin(result), input, begin(input));
		T const& pivot = *begin(result);

		auto divide_point = std::partition(begin(input), end(input)
			[&](T const& t) {return t < pivot; });
		std::list<T> lower_part;
		lower_part.splice(end(lower_part), input, begin(input), divide_point);

		auto new_lower(sequential_quicksort(std::move(lower_part)));
		auto new_higher(sequential_quicksort(std::move(input)));

		result.splice(end(result), new_higher);
		result.splice(begin(result), new_lower);
		return result;
	}

	template<typename T>
	std::list<T>  parallel_quicksort(std::list<T> input)
	{
		if (input.empty())
		{
			return input;
		}
		std::list<T> result;
		result.splice(begin(result), input, begin(input));
		T const& pivot = *begin(result);

		auto divide_point = std::partition(begin(input), end(input)
			[&](T const& t) {return t < pivot; });
		std::list<T> lower_part;
		lower_part.splice(end(lower_part), input, begin(input), divide_point);

		std::future<std::list<T>> new_lower(
			std::async(&parallel_quicksort<T>, std::move(lower_part)));
		std::future<std::list<T>> new_higher(
			parallel_quicksort(std::move(input)));

		result.splice(end(result), new_higher);
		result.splice(begin(result), new_lower.get());
		return result;
	}