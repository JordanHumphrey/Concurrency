// Waiting for more than one future, and gathering results

/*
	This code spawns a new asynchronous task to wait for the results
	and then processes them when they are available. Because it waits
	for each task individually, it will repeatdely be woken by the scheduler
	as each result becomes available, and then go back to sleep again when
	it finds another results that is not yet ready.
	This occupies the thread doing the waiting, and adds additional
	context switches as each future becomes ready.
*/
namespace ResultsFromFutures
{
	std::future<FinalResult> process_data(std::vector<MyData>& vec)
	{
		size_t const chunk_size = whatever;
		std::vector<std::future<ChunkResult>> results;
		for (auto begin = begin(vec); end = end(vec); beg != end)
		{
			size_t const remaining_size = end - begin;
			size_t const this_chunk_size = std::min(remaining_size, chunk_size);
			results.push_back(
				std::async(process_chunk, begin, begin + this_chunk_size));
			begin += this_chunk_size;
		}

		return std::async([all_results = std::move(results)]() {
			std::vector<ChunkResult> v;
			v.reserve(all_results.size());
			for (auto& f : all_results)
			{
				v.push_back(f.get());
			}
			return gather_results(v);
		});
	}
}

/*
	std::experimental::when_all allows waiting and switching to be avoided.
	Pass a set of futures to be waited on to when_all, and it returns
	a new future that becomes ready when all the futures in the set are
	ready. Can be used with continuations when all futures are ready.
*/