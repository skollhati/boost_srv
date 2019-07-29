#pragma once
#include "stdafx.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include<sstream>
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;

namespace C = std::chrono;


std::string TimepointToString(const std::chrono::system_clock::time_point& p_tpTime,
	const std::string& p_sFormat)
{
	auto converted_timep = std::chrono::system_clock::to_time_t(p_tpTime);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&converted_timep), p_sFormat.c_str());

	return oss.str();
}

class BoostLogger {
 
public: 
	BoostLogger()
	{
		
		auto tp = C::system_clock::now();
		std::string log_path("./logs/");
		
		log_path += TimepointToString(tp, "%Y%m%d");
		log_path += "/boost_srv_%N.log";
	
		logging::add_file_log
		(
			keywords::file_name = log_path,
			keywords::rotation_size = 10 * 1024 * 1024,                                   
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), 
			keywords::format = "[%TimeStamp%]: %Message%"                                 
		);

		logging::core::get()->set_filter
		(
			logging::trivial::severity >= logging::trivial::info
		);
	}

	src::severity_logger< severity_level > lg;
	


};





