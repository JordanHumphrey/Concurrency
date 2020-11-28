// Chaining continuations of time consuming tasks

// Sequential Version
void process_login(std::string const& username, std::string const& password)
{
	try {
		user_id const id = backend.authenticate_user(username, password);
		user_data const info_to_display = backend.request_current_info(id);
		update_display(info_to_display);
	}
	catch (std::exception& e) {
		display_error(e);
	}
}

/* Asynchronous Version, still not that great, it blocks the thread
 if the work is punted to the background thread, uses a lot of resources
 and just isnt good. All the threads spawned could just end up waiting
 and using up resources in a wasteful manner. */
std::future<void> process_login(
	std::string const& username, std::string const& password)
{
	return std::async(std::launch::async, [=]() {
		try {
			user_id const id = backend.authenticate_user(username, password);
			user_data const info_to_display = backend.request_current_info(id);
			update_display(info_to_display);
		}
		catch (std::exception& e) {
			display_error(e);
		}
		});
}

/*
  To avoid all the blocked threads, need mechanism for chaining
  tasks as they each complete: continuation.
  std::experimental::future only allows the stored value to be
  retrieved once, and if that value is being consumed by a
  continuation, it cannot be accessed by other code.
  You cannot pass arguments to a continuation. The function is
  passed a ready future that holds the result of that triggered
  the continuation.
*/
std::experimental::future<void> process_login(
	std::string const& username, std::string const& password)
{
	return spaw_async([=]() {
		return backend.authenticate_user(username, password);
		}).then([](std::experimental::future<user_id> id) {
			return backend.request_current_info(id.get());
			}).then([](std::experimental::future<user_data> info_to_display) {
				try {
					update_display(info_to_display.get());
				}
				catch (std::exception& e) {
					display_error(e);
				}
				});
}