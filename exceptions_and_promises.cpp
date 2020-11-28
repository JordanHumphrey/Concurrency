#include <future>

//double calculate_value();
//extern std::promise<double> some_promise;
//void example()
//{
//	// Fool proof
//	try
//	{
//		some_promise.set_value(calculate_value());
//	}
//	catch (...)
//	{
//		some_promise.set_exception(std::current_exception());
//	}
//
//	// Simpler if exception thrown is known
//	some_promise.set_exception(
//		std::make_exception_ptr(
//			std::logic_error("it died")));
//}