#include <mutex>

/*
	A std::unique_lock doesn't always own the mutex
		it's associated with. 
	Can pass std::adopt_lock as a second argument
		to constructor to have lock object manage the 
		lock on a mutex, also pass std::defer_lock to 
		indicate the mutex should remain unlocked on 
		construction.
	Lock can be acquired by calling lock on the 
		std::unique_lock object, not the mutex. 
	A std::unique_lock has a performance penalty
		when it needs to update the owns_lock flag,
		and is better used for cases where deferring
		locking is needed or ownership of a lock needs
		to be transferred from one scope to another.
	std::unique_lock is moveable but NOT copyable.
*/

class some_big_object {};
void swap(some_big_object& lhs, some_big_object& rhs);

class X
{
private:
	some_big_object some_detail;
	std::mutex m;
public:
	X(some_big_object const& sd) :some_detail(sd) {}
	friend void swap(X& lhs, X& rhs)
	{
		if (&lhs == &rhs)
			return;
		std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock); // std::defer_lock leaves
		std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock); // mutexes unlocked
		std::lock(lock_a, lock_b);
		swap(lhs.some_detail, rhs.some_detail);
	}
};

/*
	Example below describes transferring ownership between scopes:
		Because lk is an automatic variable declared within function,
		it can be returned without a need to call std::move(),
		compiler will do this on its own. Process_data() can then
		transfer ownership directly into its own std::unique_lock
		instance and then call do_something() reliably without 
		another thread altering the data.

	This sort of pattern is useful as a gateway class to ensure
		correctly lockedd access to some protected data. If any
		thread wants to access the data, you obtain an instance
		of teh gateway class by calling get_lock(). When done,
		destroy the gateway object.
*/

void prepare_data();
void do_something();
std::unique_lock<std::mutex> get_lock()
{
	extern std::mutex some_mutex;
	std::unique_lock<std::mutex> lk(some_mutex);
	prepare_data();
	return lk;
}
void process_data()
{
	std::unique_lock<std::mutex> lk(get_lock());
	do_something();
}

/*
	When multiple threads are waiting for the same resource,
	if any thread holds the lock for longer than necessary,
	unlock it and relock it when ready and needed.
*/

class some_class {};
some_class get_next_data_chunk();
int process(some_class s);
void write_result(some_class s, int r);
void get_and_process_data()
{
	extern std::mutex some_mutex;
	std::unique_lock<std::mutex> my_lock(some_mutex);
	some_class data_to_process = get_next_data_chunk();
	my_lock.unlock(); // Don't need mutex locked across call to process()
	int result = process(data_to_process);
	my_lock.lock();	  // Relock mutex to write result
	write_result(data_to_process, result);
}