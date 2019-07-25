#pragma once
#include "stdafx.h"
#include "boost/asio.hpp"

using namespace boost;

class BoostServer {

public:
	BoostServer(std::string ip_addr, unsigned short port);
	
	~BoostServer()
	{}

	void ServerBegin();


private:
	asio::ip::address ip_address;
	asio::ip::tcp::endpoint endpoint;
	system::error_code ec;
	


};