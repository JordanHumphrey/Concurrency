#pragma once
#include <future>
#include <iostream>>
int find_the_answer__to_ltuae();
void do_other_stuff();

int main()
{
	std::future<int> the_answer = std::async(find_the_answer__to_ltuae);
	do_other_stuff();
	// Have faith the answer is 42
	std::cout << "The answer is " << the_answer.get() << std::endl;
}

// Passing arguments to a function with std::async
struct X
{
	void foo(int, std::string const&);
	std::string bar(std::string const&);
};
X x;
// So long AOL messenger =(
auto aol1 = std::async(&X::foo, &x, 42, "hello");
auto aol2 = std::async(&X::bar, &x, "goodbye");
X baz(X&);

struct Y
{
	double operator() (double);
};

// Launch a new thread with async, rather than synchronusly when future is waited on
auto f6 = std::async(std::launch::async, Y(), 1.2);

// Run in wait() or get()
auto f7 = std::async(std::launch::deferred, baz, std::ref(x));

// Implementation chooses
auto f8 = std::async(
	std::launch::deferred | std::launch::async,
	baz, std::ref(x));

// Invoke deferred function
void temp()
{
	f7.wait();
}