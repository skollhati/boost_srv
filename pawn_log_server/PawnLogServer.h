#pragma once


#include <boost/make_shared.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/system/error_code.hpp>
#include "ServerSession.h"

using namespace boost::asio::ip;
class BoostNetworkServer
{
public:

	BoostNetworkServer();
	BoostNetworkServer(int port_number) :p_EndPoint(tcp::v4(), port_number), m_Accepter(m_IoContext, p_EndPoint)
	{
		try_accept();
	}


	~BoostNetworkServer();



private:
	void try_accept()
	{
		m_Accepter.async_accept(
			[this](boost::system::error_code ec, tcp::socket socket)
			{
				if (!ec)
				{
					boost::make_shared<ServerSession>(std::move(socket));
				}
				//접속 실패 다음 accept 시도
				try_accept();
			}
		);
	}

	boost::asio::io_context m_IoContext;
	tcp::endpoint p_EndPoint;
	tcp::acceptor m_Accepter;


};