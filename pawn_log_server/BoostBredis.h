#pragma once
#include "bredis/Connection.hpp"
#include "bredis/Extract.hpp"
#include "bredis/MarkerHelpers.hpp"
#include "boost/variant.hpp"
#include "boost/asio/basic_stream_socket.hpp"
#include "boost/asio.hpp"
#include <memory>
#include "stdafx.h"

namespace r = bredis;
namespace asio = boost::asio;

using socket_t = asio::ip::tcp::socket;
using Buffer = boost::asio::streambuf;
using Iterator = typename r::to_iterator<Buffer>::iterator_t;


class BoostBredis {

public:
	boost::system::error_code ec;
	boost::shared_ptr<r::Connection<socket_t>> c;
	asio::io_context ioContext;

	Buffer rxBuff;


	boost::shared_ptr<socket_t> socket;
	BoostBredis()
	{

	}

	void SetBredis(std::string hostAddr = "127.0.0.1", int port = 6379, std::string requirepass = "123456")
	{

		asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(hostAddr), port);

		socket = boost::make_shared<socket_t>(ioContext, endpoint.protocol());
		socket->connect(endpoint);
		c = boost::make_shared<r::Connection<socket_t>>(std::move(*socket));

		if (!auth_redis(requirepass).compare("OK"))
		{

		}

	}

	void ResetBredis()
	{
		rxBuff.consume(rxBuff.size());
	}

	std::string get_by_key(std::string key)
	{

		c->write(r::single_command_t{ "get",key }, ec);

		return read_buffer();
	}

	std::string auth_redis(std::string requirepass)
	{
		if (!requirepass.empty())
		{
			c->write(r::single_command_t{ "auth", requirepass });

			return read_buffer();
		}

		return "OK";
	}

	std::string set(std::string key, std::string value)
	{
		c->write(r::single_command_t{ "set", key, value });

		return read_buffer();

	}

	std::string commnad(std::string cmd, std::array<std::string, 5> args)
	{
		auto cmdCon = r::single_command_t{ cmd };

		for (auto& var : args)
		{
			cmdCon.arguments.push_back(var);
		}

		c->write(cmdCon);

		return read_buffer();
	}

	std::string read_buffer()
	{

		auto result_markers = c->read(rxBuff);
		auto extract = boost::apply_visitor(r::extractor<Iterator>(), result_markers.result);
		//rxBuff.consume(rxBuff.size());
		rxBuff.consume(result_markers.consumed);

		return boost::get<r::extracts::string_t>(extract).str;
	}
};

