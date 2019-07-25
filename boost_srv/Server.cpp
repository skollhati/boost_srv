#include "Server.h"


BoostServer::BoostServer(std::string ip_addr, unsigned short port)
{
	this->ip_address = asio::ip::address::from_string(ip_addr, this->ec);

	if (this->ec.value() != 0)
	{

	}

}