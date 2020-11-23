
//void hello() {
//	//std::cout << "hello" << std::endl;
//}
//
//class background_task
//{
//public:
//	void operator()() const
//	{
//		
//	}
//};
//background_task f;
//std::thread thread1(f);
//std::thread thread2(background_task());// does not spawn new thread
//std::thread thread3{ background_task() }; // spawns new thread
//std::thread thread4([]() {
//	hello();
//	});
//
//#pragma region A function that returns while a thread still has access to local variables
//struct func
//{
//	int& i;
//	func(int& i_) :i(i_) {}
//	void operator () ()
//	{
//		for (unsigned j = 0; j < 1000000; j++)
//		{
//			hello(); // <- potential access to dangling reference
//		}
//	}
//};
//
//void oops()
//{
//	int some_local_state = 0;
//	func myfunc(some_local_state);
//	std::thread my_thread(myfunc);
//	my_thread.detach(); // dont wait for thread to finish
//} // new thread might still be running
//#pragma endregion
//
//#pragma region Waiting for a thread to finish try/catch
//struct func2
//{
//	void f()
//	{
//		int some_local_state = 0;
//		func my_func(some_local_state);
//		std::thread t(my_func);
//		try
//		{
//			void();
//		}
//		catch (...)
//		{
//			t.join();
//			throw;
//		}
//		t.join();
//	}
//};
//#pragma endregion
//
//#pragma region Using RAII to wait for a thread to complete
//class thread_guard
//{
//	std::thread& t;
//public:
//	explicit thread_guard(std::thread& t_) :
//		t(t_) {}
//	~thread_guard()
//	{
//		if (t.joinable())
//			t.join();
//	}
//	// make copy and copy assignment functions as delete
//	// so that compiler wont make them. If a thread_guard
//	// object is copied, it could lead to all sorts of errors.
//	thread_guard(thread_guard const&) = delete;
//	thread_guard& operator=(thread_guard const&) = delete;
//};
//struct func3
//{
//	void f()
//	{
//		int some_local_state = 0;
//		func my_func(some_local_state);
//		std::thread t(my_func);
//		thread_guard g(t);
//		void();
//	}
//};
//#pragma endregion
//
//#pragma region Detaching a thread to handle other documents
//void edit_document(std::string const& filename)
//{
//	//open_document_and_display_gui(filename);
//	//while (!done_editing())
//	//{
//	//	user_command cmd = get_user_input();
//	//	if (cmd.type == open_new_document)
//	//	{
//	//		std::string const new_name = get_file_name_from_user();
//	//		std::thread t(edit_document, new_name);
//	//		t.detach();
//	//	}
//	//	else
//	//	{
//	//		process_user_input(cmd);
//	//	}
//	//}
//}
//#pragma endregion
//
//#pragma region Passing Arguments to a thread Function
//// this creates a new thread of execution with t, as rvalues
//// and the string literal is passed as a char const*
//void f1(int i, std::string const& s);
//std::thread t1(f, 3, "hello");
//
//void f2(int i, std::string const& s);
//void oops(int some_param)
//{
//	/*
//	*	the pointer to the local variable buffer thats passed
//	* through to the new thead and there is a good chance the 
//	* oops function will exit before buffer has been converted
//	* to a std::String on the new thread, leading to undefined 
//	* behavior. Solution is to cast before passing to avoid 
//	* the dangling pointer.
//	*/
//	char buffer[1024];
//	sprintf(buffer, "%i", some_param);
//	//std::thread t(f, 3, buffer);
//	std::thread t(f, 3, std::string(buffer));
//	t.detach();
//}
//
//// another scenario
///*
//	Because the constructor of a thread blindly copies data
//	over as rvalues, it doesnt give a damn about what you
//	wanted to be passed along as reference. It will fail to compile.
//	A way to get around this is to bind the reference variable
//	explicitly with std::ref(data), and will be correctly passed
//	as a reference to data rather than a temp copy of data, and compile.
//*/
////void update_data_for_widget(widget_id w, widget_data& data);
////void oops_again(widget_id w)
////{
////	widget_data data;
////	std::thread t(update_data_for_widget, w, std::ref(data));
////	display_status();
////	t.join();
////	process_widget_data(data);
////}
//#pragma endregion
//
//#pragma region Passing member function pointer as the function
//// could be useful for just calling a single method needed
//// that does a lot of processing when u only need that one
//// function to be called (maybe for detached?)
//// can also pass arguments to the function
//class X
//{
//public:
//	void do_something(std::string const& s);
//};
//X my_x;
//std::thread t(&X::do_something, &my_x, std::string("hello"));
//#pragma endregion
//
//#pragma region Move Semantics with Unique_Ptr
///*
//By specifying std::move(p) in the std::thread constructor, the
//ownership of big_object is transferred first into internal storage
//for the newly created thread and then into process_big_object
//*/
//struct big_object { void prepare_data(int data); };
//void process_big_object(std::unique_ptr<big_object>);
//std::unique_ptr<big_object> p(new big_object);
//std::thread t(process_big_object, std::move(p));
//#pragma endregion
//
//#pragma region Transferring ownership of a thread
///*
//instances of a thread are movable but NOT copyable
//*/
//void some_function();
//void some_other_function();
//void example()
//{
//	std::thread t1(some_function);
//	std::thread t2 = std::move(t1);
//	t1 = std::thread(some_other_function);
//	std::thread t3;
//	t3 = std::move(t2);
//	t1 = std::move(t3); // terminates the program
//}
//#pragma endregion
//
//#pragma region Returning a thread from a function
//std::thread foo() {
//	void some_function();
//	return std::thread(some_function);
//}
//
//std::thread g() {
//	void some_other_function();
//	std::thread t(some_other_function);
//	return t;
//}
//
///*
//	Ownership of a function can be transferred into a function
//*/
//void fo(std::thread t);
//void go()
//{
//	void some_function();
//	fo(std::thread(some_function));
//	std::thread t(some_function);
//	fo(std::move(t));
//}
//#pragma endregion
//
//#pragma region scoped_thread and example
//void do_something_in_current_thread();
//class scoped_thread
//{
//	std::thread t;
//public:
//	explicit scoped_thread(std::thread t_) :
//		t(std::move(t_))
//	{
//		if (!t.joinable())
//			throw std::logic_error("No thread");
//	}
//	~scoped_thread()
//	{
//		t.join();
//	}
//	scoped_thread(scoped_thread const&) = delete;
//	scoped_thread& operator=(scoped_thread const&) = delete;
//};
//void funner()
//{
//	int some_local_state;
//	scoped_thread t{std::thread(func(some_local_state))};
//	do_something_in_current_thread();
//}
//#pragma endregion
//
//#pragma region a joining_thread class
///*

//#pragma endregion
//
//#pragma region spawn threads and wait return
//void do_work(unsigned id);
//void temp()
//{
//	std::vector<std::thread> threads;
//	for (unsigned i = 0; i < 20; ++i) {
//		threads.emplace_back(do_work, i); // spawns threads
//	}
//	for (auto& entry : threads)
//		entry.join();
//}
//#pragma endregion
//
//#pragma region choosing number of threads at runtime
//// returns an indication of the number of threads that can
//// be truly run concurrently for a given execution of a program
//// On a multicore sytstem, it might be number of CPU cores
//// Returns 0 if info not available
//int num_threads = std::thread::hardware_concurrency();
//
//// parallel version of std::accumulate
//template<typename Iterator, typename T>
//struct accumulate_block
//{
//	void operator() (Iterator first, Iterator last, T& result)
//	{
//		return = std::accumulate(first, last, result);
//	}
//};
//template<typename Iterator, typename T>
//T parallel_accumulate(Iterator first, Iterator last, T init)
//{
//	unsigned long const length = std::distance(first, last);
//	if (!length)
//		return init;
//	unsigned long const min_per_thread = 25;
//	unsigned long const max_threads =
//		(length + min_per_thread - 1) / min_per_thread;
//	unsigned long const hardware_threads =
//		std::thread::hardware_concurrency();
//	unsigned long const num_threads =
//		std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
//	unsigned long const block_size = length / num_threads;
//	std::vector<T> results(num_threads);
//	std::vector<std::thread> threads(num_threads - 1);
//	Iterator block_start = first;
//	for (unsigned long i = 0; i < (num_threads - 1); ++i)
//	{
//		Iterator block_end = block_start;
//		std::advance(block_end, block_size);
//		threads[i] = std::thread(
//			accumulate_block<Iterator, T>(),
//			block_start, block_end, std::ref(results[i]));
//		block_start = block_end;
//	}
//	accumulate_block<Iterator, T>() (
//		block_start, last, results[num_threads - 1]);
//
//	for (auto& entry : threads)
//		entry.join();
//	return std::accumulate(begin(results), end(results), init);
//}
//#pragma endregion
//
//#pragma region Protecting a list with a mutex
//#include <list>
//#include <mutex>
//std::list<int> some_list;
//std::mutex some_mutex;
//void add_to_list(int new_value)
//{
//	std::lock_guard<std::mutex> guard(some_mutex);
//	some_list.push_back(new_value);
//}
//bool list_contains(int value_to_find)
//{
//	std::lock_guard<std::mutex> guard(some_mutex);
//	return std::find(begin(some_list), end(some_list), value_to_find)
//		!= end(some_list);
//}
//#pragma endregion