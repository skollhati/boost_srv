#pragma once
#include "stdafx.h"

#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/record_ordering.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include<sstream>
#include <string>
#include <fstream>

namespace attrs = boost::log::attributes;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
using namespace logging::trivial;


#define CUSTOM_LOG_TRACE(logger,sev) \
BOOST_LOG_SEV(logger, sev) <<"["<< sev <<"]" << "(" << __FILE__ << ", " << __LINE__ << ") " << ":"



BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(global_lg, src::logger_mt)

class BoostLogger {
 
public: 
	
	typedef sinks::text_ostream_backend backend_t;
	typedef sinks::text_file_backend file_sink;
	typedef sinks::asynchronous_sink<sinks::text_file_backend, sinks::unbounded_ordering_queue
		<logging::attribute_value_ordering<unsigned int, std::less<unsigned int>>>> sink_t;
	boost::shared_ptr<sink_t> sink;
	boost::shared_ptr<logging::core> core;
	BoostLogger() = default;
	BoostLogger(std::string fileName)
	{
		try 
		{
			core = logging::core::get();
			
			sink = boost::make_shared<sink_t>(boost::make_shared<file_sink>(), 
				keywords::order = logging::make_attr_ordering("RecordID", std::less<unsigned int>()));
			
			//Log 파일 설정
			sink->locked_backend()->set_file_collector(sinks::file::make_collector(keywords::target = "logs"));
			sink->locked_backend()->set_rotation_size(1024*10*10);
			sink->locked_backend()->set_time_based_rotation(sinks::file::rotation_at_time_point(0, 0, 0));
			sink->locked_backend()->set_open_mode(std::ios_base::out | std::ios_base::app);
			sink->locked_backend()->set_file_name_pattern(fileName+"_%Y%m%d_%N.log");
			sink->locked_backend()->scan_for_files();
	
			//포멧 설정
			sink->set_formatter(
				expr::stream
				<< "<" << expr::attr<boost::thread::id>("ThreadID") << ">"
				<< "[" << expr::attr<unsigned int>("RecordID") << "]"
				<< "[" 
				<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
				<< "]"
				<< expr::smessage
			);

			//core->add_sink(sink);
			//core->add_global_attribute("RecordID", attrs::counter<unsigned int>());
			//core->add_global_attribute("TimeStamp", attrs::local_clock());
			
		}
		catch (std::exception& e)
		{
			std::cout << "FAILURE:" << e.what() << std::endl;
		}
		
	}

	~BoostLogger()
	{
		core->remove_sink(sink);
		
		sink->stop();
		sink->flush();
		sink.reset();
		
	}
		


	
	boost::shared_ptr<sink_t> getSink()
	{
		return sink;
	}


};

//expr::attr<boost::posix_time::ptime>("TimeStamp") 
//sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>());
			//sink->locked_backend()->set_file_collector(sinks::file::make_collector(keywords::target = "logs/%Y%m%d"));


	//keywords::target_file_name = "./logs/"+ TimepointToString(tp, "%Y%m%d") +"/boost_srv_%Y%m%d_%N.log",
			//	keywords::max_size = 1024 * 10,
				//keywords::scan_method = sinks::file::scan_matching,//파일 확인
				//keywords::open_mode = std::ios_base::out | std::ios_base::app 