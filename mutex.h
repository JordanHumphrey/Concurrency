#include <list>
#include <mutex>
#include <thread>
#include <algorithm>

// Also available is std::scoped_lock

class some_list
{
private:
	std::list<int> some_list;
	std::mutex some_mutex;
public:
	void add_to_list(int new_value)
	{
		//std::lock_guard<std::mutex> guard(some_mutex);
		std::lock_guard guard(some_mutex);
		some_list.push_back(new_value);
	}

	bool list_contains(int value_to_find)
	{
		//std::lock_guard<std::mutex> guard(some_mutex);
		std::lock_guard guard(some_mutex);
		return std::find(begin(some_list), end(some_list), value_to_find)
			!= end(some_list);
	}

	bool try_lock()
	{
		if (some_mutex.try_lock())
			return true;
		else
			return false;
	}
};