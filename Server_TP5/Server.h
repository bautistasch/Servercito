#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include <string>
#include <istream>

#include "AbstractProtocol.h"

class Server {
public:
	Server(boost::asio::io_context& context, AbstractProtocol * a);
	void startListening();
	AbstractProtocol * abstractProtocol;
private:
	void start_answering();
	void start_reading();
	void connection_received_cb(const boost::system::error_code& error);
	void response_sent_cb(const boost::system::error_code& error, size_t bytes_sent);
	void response_recived_cb(const boost::system::error_code& error, std::size_t size);

	std::string dataRead;

	boost::asio::streambuf buffer;

	std::string msg;

	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::acceptor acceptor_;
};
