#include <iostream>
#include <boost/asio.hpp>
#include "Server.h"

#include "Http.h"

#include <fstream>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	//try
	//{
	//	boost::asio::io_context io_context;

	//	Server server(io_context);

	//	server.startListening();

	//	io_context.run();
	//}
	//catch (std::exception& e)
	//{
	//	std::cerr << e.what() << std::endl;
	//}


	//std::ifstream myReadFile;
	//std::string myString("");
	//myReadFile.open("toRead.txt");
	//if (myReadFile.is_open()) {
	//	while (!myReadFile.eof()) {
	//		myString += myReadFile.get();
	//	}
	//	std::cout << myString << std::endl;
	//	std::cout << myString.size() << std::endl;
	//	std::string caca("hola");
	//	std::cout << caca.size() << std::endl;
	//}

	//Http myhttp;

	//std::string request = std::string("GET ") + " HTTP/1.1   " + "/toRead.txt" + "\r\n" +
	//							"Host: 127.0.0.1" + "\r\n" +
	//							"al pedo al pedo al pedo" +"\r\n" +
	//							"\r\n";

	//std::cout << myhttp.getAnswer(request) << std::endl;

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