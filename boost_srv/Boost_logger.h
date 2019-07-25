#pragma once
#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"
#include "boost/log/sources/record_ostream.hpp"
#include "boost/log/sources/severity_logger.hpp"
#include "stdafx.h"


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;


//Singleton
class BoostLogger {
 
public: 
	BoostLogger()
	{
		logging::add_file_log(
			keywords::file_name = "boost_srv_%d %H:%M:%S_%n.log",
			keywords::rotation_size = 10 * 1024 * 1024,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::format = "[%TimeStamp%] : %Message%"
		);
		
		logging::core::get()->set_filter(
			logging::trivial::severity >= logging::trivial::info
		);

		this->StartLog();

	}
	
	void StartLog()
	{
		logging::add_common_attributes();
		
	}

public :
	src::severity_logger<severity_level> lg;

};





