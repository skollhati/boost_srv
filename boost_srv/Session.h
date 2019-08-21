#pragma once
#include "stdafx.h"

enum STATE {
	UNAUTH,
	LOGIN,
	LOBBY,
	MARKET,
	WAITING,
	GAME
};

typedef struct USERINFO
{
	std::string name;
	std::string user_id;
	std::string user_grade;
	int user_level;
	int auth_idx;
	STATE state;
};

class UserSession
{
public:
	UserSession();
	virtual ~UserSession();

	void ChangeState(STATE state)
	{
		m_UserInfo.state = state;
	}


private:
	USERINFO m_UserInfo;
	   	 

};