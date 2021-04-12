#include <iostream>

#include "Server.h"


#include <string>
#include <cstdio>


using boost::asio::ip::tcp;


Server::Server(boost::asio::io_context& io_context, AbstractProtocol * abst)
	: context_(io_context),
	acceptor_(io_context, tcp::endpoint(tcp::v4(), 80)),
	socket_(io_context),
	abstractProtocol(abst)
{
}

void Server::startListening()
{
	std::cout << "start_waiting_connection()" << std::endl;
	if (socket_.is_open())
	{
		std::cout << "Error: Can't accept new connection from an open socket" << std::endl;
		return;
	}
	acceptor_.async_accept(			//solo recibe socket que va a administrar la nueva conexion y el callback
		socket_,
		boost::bind(
			&Server::connection_received_cb,
			this,
			boost::asio::placeholders::error
		)
	);
}

void Server::connection_received_cb(const boost::system::error_code& error)
{
	std::cout << "connection_received_cb()" << std::endl;
	if (!error) {
		//start_answering();
		//startListening();
		start_reading();
	}
	else {
		std::cout << error.message() << std::endl;
	}
}

void Server::start_answering()
{
	std::cout << "start_answering()" << std::endl;
	//msg = "OK, RECIBI CONEXION SOY SERVER";
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(msg),
		boost::bind(
			&Server::response_sent_cb,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
	//start_reading();

}

void Server::start_reading()
{
	std::cout << "start_reading()" << std::endl;

	boost::asio::async_read_until(socket_, buffer, "\r\n\r\n",
		boost::bind(&Server::response_recived_cb, this, boost::asio::placeholders::error));
}

void Server::response_recived_cb(const boost::system::error_code& error)
{
	std::istream is(&buffer);
	
	std::getline(is, dataRead, {});

	std::cout << "HE RECIBIDO: " << dataRead << std::endl;
	std::cout << "SIZE DE LA DATA QUE ME LLEGO:" << dataRead.size() << std::endl;

	msg = abstractProtocol->getAnswer(dataRead);

	start_answering();
}


void Server::response_sent_cb(const boost::system::error_code& error,
	size_t bytes_sent)
{
	std::cout << "response_sent_cb()" << std::endl;
	if (!error) {
		std::cout << "Response sent. " << bytes_sent << " bytes." << std::endl;
	}
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	socket_.close();
}



