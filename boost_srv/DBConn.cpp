#include "DBConn.h"
#include "PoolConnObj.h"
using namespace std;
MYSQLCONN::MYSQLCONN()
{

}

MYSQLCONN::MYSQLCONN(std::string user_id, std::string password, std::string host, std::string schema,int pool_size)
{
	try {
		
		for (int i = 0; i < pool_size; i++)
		{
			this->pool.push_back(new PoolConnObj(new Session(SessionOption::HOST, host, SessionOption::PORT, 6002,
				SessionOption::USER, user_id, SessionOption::PWD, password)));
		}
	}
	catch(const mysqlx::Error &err){
		

	}
	cout << "max_pool_size : " << pool_size << endl;
	cout << "generated cnt :" << this->pool.capacity << endl;
	cout << "pool setting is complete" << endl;

}


Session MYSQLCONN::GetConnection()
{
	while (1)
	{

	}
}




