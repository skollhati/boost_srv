#pragma once
#include "stdafx.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>

using namespace boost::asio::ip;

enum STATE {
	UNAUTH,
	LOGIN,
	LOBBY,
	MARKET,
	WAITING,
	LOADING,
	GAME,
	ADJUSTMENT
};

typedef struct USERINFO
{
	std::string name;
	std::string user_id;
	int user_grade;
	int user_level;
	int auth_idx;
	
};

class UserSession
{
public:
	UserSession() = default;
	
	UserSession(tcp::socket socket):m_Socket(std::move(socket))
	{
		
	}

	virtual ~UserSession();
	
	void SetUserInfo(USERINFO& userInfo)
	{
		m_UserInfo = userInfo;
	}
	
	int ChangeState(STATE state)
	{
		
		//redis¿¡ ³Ö±â
		return 0;
	}

	STATE GetNowState()
	{

	}



private:
	USERINFO m_UserInfo;
	tcp::socket m_Socket;
	
};