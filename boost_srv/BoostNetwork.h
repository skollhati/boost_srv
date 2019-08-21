#pragma once
#include "stdafx.h"

#include <boost/make_shared.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/asio/ip/address.hpp>
using namespace boost::asio::ip;
class BoostNetworkServer
{
public:

	
	BoostNetworkServer();
		
	~BoostNetworkServer();
	

private:

	boost::asio::io_context m_IoContext;
	tcp::socket m_Socket;
	tcp::acceptor m_Accepter;
	 	

};