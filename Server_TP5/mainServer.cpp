#include <iostream>
#include <boost/asio.hpp>
#include "Server.h"

#include "Http.h"

#include <fstream>

using boost::asio::ip::tcp;

int main()
{
	try
	{
		boost::asio::io_context io_context;

		Http httpProtocol;

		Server server(io_context, &httpProtocol);

		server.startListening();

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}