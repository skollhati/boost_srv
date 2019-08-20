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

BoostBredis* InitSetBoostBredis(BoostBredis* b)
{
	b->SetBredis();
	
	return b;
}

BoostBredis* ResetBoostBredis(BoostBredis* b)
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
	BoostLogger boostLogger;
    
	//logging::add_common_attributes();
	using namespace logging::trivial;

	// Add it to the core
	logging::core::get()->add_sink(boostLogger.getSink());

	// Add some attributes too
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
	logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());

	// Create logging threads
	boost::barrier bar(THREAD_COUNT);
	boost::thread_group threads;
	for (unsigned int i = 0; i < THREAD_COUNT; ++i)
		threads.create_thread(boost::bind(&thread_fun, boost::ref(bar)));

	// Wait until all action ends
	threads.join_all();
	
	CustomObjectPool<BoostBredis>* cop ;
	cop->instance(5, 10, &InitSetBoostBredis, &ResetBoostBredis);


}



//BOOST_LOG_SCOPED_THREAD_TAG("ThreadID", boost::this_thread::get_id());
//BOOST_LOG_SEV(global_lg::get(), trace) << "A trace severity message";
//BOOST_LOG_SEV(global_lg::get(), debug) << "A debug severity message";
//BOOST_LOG_SEV(global_lg::get(), info) << "An informational severity message";
//BOOST_LOG_SEV(global_lg::get(), warning) << "A warning severity message";
//BOOST_LOG_SEV(global_lg::get(), error) << "An error severity message";
//BOOST_LOG_SEV(global_lg::get(), fatal) << "A fatal severity message";
//LFC1_LOG_TRACE(global_lg::get(),error)<<"test";
