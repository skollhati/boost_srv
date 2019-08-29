// boost_srv.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "bredis/Extract.hpp"
#include "bredis/MarkerHelpers.hpp"
#include "boost/iostreams/stream_buffer.hpp"
#include "boost/asio/streambuf.hpp"
#include "stdafx.h"
#include "Boost_logger.h"
#include "BoostBredis.h"
#include "CustomObjectPool.h"

boost::shared_ptr<BoostBredis> InitSetBoostBredis(boost::shared_ptr<BoostBredis> b)
{
	b->SetBredis();
	
	return b;
}

boost::shared_ptr<BoostBredis> ResetBoostBredis(boost::shared_ptr<BoostBredis> b)
{
	b->ResetBredis();
	return b;
}

enum
{
	LOG_RECORDS_TO_WRITE = 10000,
	THREAD_COUNT = 2
};

void thread_fun(boost::barrier& bar)
{
	// Wait until all threads are created
	bar.wait();

	// Here we go. First, identify the thread.
	BOOST_LOG_SCOPED_THREAD_TAG("ThreadID", boost::this_thread::get_id());

	// Now, do some logging
	for (unsigned int i = 0; i < LOG_RECORDS_TO_WRITE; ++i)
	{
		CUSTOM_LOG_TRACE(global_lg::get(), warning)  << "Log record " << i;
	}
}
int main()
{
	using namespace logging::trivial;
	BoostLogger boostLogger;
	
	

	// Add it to the core
	logging::core::get()->add_sink(boostLogger.getSink());

	// Add some attributes too
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
	logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
	
	auto co = CustomObjectPool<BoostBredis>::instance(5, 10, InitSetBoostBredis, ResetBoostBredis);

	std::cout << "test";


}




