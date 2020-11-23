#pragma once
#include <mutex>
#include <queue>

// Beginner way to do it

bool flag;
std::mutex m;
void wait_for_flag()
{
	std::unique_lock<std::mutex> lk(m);
	while (!flag)
	{
		lk.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		lk.lock();
	}
}

// Better way to do it with condition variable

std::mutex mut;
std::queue<int> data_queue;
std::condition_variable data_cond;
bool more_data_to_prepare();
int prepare_data();
void process(int d);
bool is_last_chunk(int d);

void data_preparation_thread()
{
	while (more_data_to_prepare())
	{
		int const data = prepare_data();
		{
			std::lock_guard<std::mutex> lk(mut);
			data_queue.push(data);
		}
		// Unlock mutex before notifying waiting thread
		data_cond.notify_one();
	}
}

void data_processing_thread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(
			lk, [] {return !data_queue.empty(); });
		int data = data_queue.front();
		data_queue.pop();
		lk.unlock();
		process(data);
		if (is_last_chunk(data))
			break;
	}
}