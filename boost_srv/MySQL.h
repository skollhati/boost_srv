#pragma once
#include "stdafx.h"


using namespace mysqlx;



class MySQLConn {
public:
	Client cli;

	MySQLConn():cli("user:password@host_name/db_name", ClientOption::POOL_MAX_SIZE, 7)
	{
		
	}
	
	Session GetConn()
	{
		return cli.getSession();
	}

	Row GetData()
	{

	}
};
