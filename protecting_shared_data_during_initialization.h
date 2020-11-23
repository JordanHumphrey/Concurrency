#pragma once
#include <memory>
#include <mutex>

// The following two examples are how not to do it

// Example 1
typedef int some_resource;
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;
void foo()
{
	std::unique_lock<std::mutex> lk(resource_mutex);
	if (!resource_ptr)
	{
		resource_ptr.reset(new some_resource);
	}
	lk.unlock();
	//resource_ptr->do_something();
}

// Example 2

/*
	this one is very nasty because it has a race condition
	that is brutal. The outside read is not synchronized
	with the inside lock held by another thread for write,
	a data race.
*/
void undefined_behavior_with_double_checked_locking()
{
	if (!resource_ptr)
	{
		std::lock_guard<std::mutex> lk(resource_mutex);
		if (!resource_ptr)
		{
			resource_ptr.reset(new some_resource);
		}
	}
	//source_ptr->do_something();/
}

/*
	Solution to exmaple 2 is std::once_flag and std::call_once,
		which ensures that whoever (thread) has the lock, will only
		call .reset() once on the pointer. Once_cal has lower overhead
		than mutex. 
	std::once_flag cannot be copied of moved. If you need to,
		you need to write them explicitly as member functions.
*/
typedef int connection_info, data_packet;
struct connection_Manager
{
public:
	connection_handle open(connection_info);
};
struct connection_handle
{
public:
	connection_handle send_data(data_packet);
	data_packet receive_data();
};
connection_Manager connection_manager;

class X
{
private:
	connection_info connection_details;
	connection_handle connection;
	std::once_flag connection_init_flag;
	
	void open_connection()
	{
		connection = connection_manager.open(connection_details);
	}
public:
	X(connection_info const& connection_details_) :
		connection_details(connection_details_)
	{}
	void send_data(data_packet const& data)
	{
		std::call_once(connection_init_flag, &X::open_connection, this);
		connection.send_data(data);
	}
	data_packet receive_Data()
	{
		std::call_once(connection_init_flag, &X::open_connection, this);
		return connection.receive_data();
	}
};