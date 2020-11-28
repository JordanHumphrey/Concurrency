//#include <future>
//#include <vector>
//
//typedef int payload_type;
//typedef int outgoing_data;
//
//struct data_packet
//{
//	int id;
//	payload_type payload;
//	std::promise<bool> promise;
//};
//
//struct connection
//{
//public:
//	data_packet incoming();
//	bool has_incoming_data();
//	bool has_outgoing_data();
//	std::promise<payload_type>& get_promise(int d);
//	data_packet top_of_outgoing_queue();
//	void send(payload_type p);
//};
//
//std::vector<connection> connection_set;
//bool done(std::vector<connection>&);
//
//void process_connections(std::vector<connection>& connections)
//{
//	while (!done(connections)) // (1)
//	{
//		for (auto connection_b = begin(connections), // (2)
//			connection_e = end(connections);
//			connection_b != connection_e;
//			++connection_b)
//		{
//			if (connection_b->has_incoming_data()) // (3)
//			{
//				data_packet data = connection_b->incoming();
//				std::promise<payload_type>& p =
//					connection_b->get_promise(data.id); // (4)
//				p.set_value(data.payload);
//			}
//			if (connection_b->has_outgoing_data()) // (5)
//			{
//				data_packet out_data =
//					connection_b->top_of_outgoing_queue();
//				connection_b->send(out_data.payload);
//				out_data.promise.set_value(true); // (6)
//			}
//		}
//		
//	}
//}
//
///*
//	Check each connection in turn(2), retrieving incoming data
//		if there is any(3) or sending and queued outgoing data(5).
//	The ID is mapped to a std::promise by a lookup in an associate 
//		container (4) and the value is set to the packets payload.
//	Once the send has completed, the promise associated with the 
//		outgoing data is set to true to indicate successful transmission.
//	Implementation will depend on the protocol
//*/