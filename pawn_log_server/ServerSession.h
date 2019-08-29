#pragma once
#include "stdafx.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>

using namespace boost::asio::ip;

enum ServerType {
	GameServer,
	ChattingServer
};

typedef struct SERVER
{
	std::string ip;
	ServerType m_serverType;
};

class ServerSession
{
public:
	ServerSession() = default;

	ServerSession(tcp::socket socket) :m_Socket(std::move(socket))
	{

	}

	virtual ~ServerSession();

	void SetUserInfo(SERVER& serverInfo)
	{
		m_ServerInfo = serverInfo;
	}





private:
	SERVER m_ServerInfo;
	tcp::socket m_Socket;

};