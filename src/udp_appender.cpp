/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"

#include "log4boost/appender/udp_appender.hpp"

#include "log4boost/detail/stack_ostream.hpp"
#include "log4boost/layout_factory.hpp"
#include "ptree_inc.h"

#include <stdexcept>
#include <boost/asio.hpp>


namespace log4boost
{

	class udp_appender_imp
	{
	public:
		udp_appender_imp(void);
		~udp_appender_imp(void);

		void open(const std::string& host,uint16_t port);

		void close();

		void append(const char* str,std::size_t length);

	private:
		boost::asio::io_service		io_service_;
		boost::asio::ip::udp::socket	socket_;
		boost::asio::ip::udp::resolver	resolver_;
		boost::asio::ip::udp::endpoint	endpoint_;
	};


	udp_appender_imp::udp_appender_imp()
		:socket_(io_service_)
		,resolver_(io_service_)
	{
	}

	udp_appender_imp::~udp_appender_imp(void)
	{
		close();
	}

	void udp_appender_imp::open(const std::string& host,uint16_t port)
	{
		using namespace boost::asio;
		ip::udp::resolver::query query(ip::udp::v4(),host,boost::lexical_cast<std::string>(port));
		ip::udp::resolver::iterator iterator = resolver_.resolve(query);
		if(iterator!=ip::udp::resolver::iterator())
		{
			endpoint_ = *iterator;
			socket_.open(ip::udp::v4());
		}
		else
		{
			throw std::runtime_error("host resolver fail!");
		}
	}

	void udp_appender_imp::append(const char* str,std::size_t length)
	{
		try
		{
			socket_.send_to(boost::asio::buffer(str,length),endpoint_);
		}
		catch(...)
		{
		}
	}

	void udp_appender_imp::close()
	{
		boost::system::error_code ec;
		socket_.close( ec );
	}


	udp_appender::udp_appender(const std::string& name,layout* l,const std::string& host
		,uint16_t port)
		:layout_appender(name,l)
		,host_(host)
		,port_(port)
	{
	}

	udp_appender::~udp_appender(void)
	{
		close();
	}

	void udp_appender::reopen()
	{
		try
		{
			close();
			if(imp_.get()==0)
			{
				imp_.reset(new udp_appender_imp());
				imp_->open(host_,port_);
			}
		}
		catch(...)
		{
			throw;
		}
	}

	void udp_appender::close()
	{
	}

	void udp_appender::_append(const logging_event& event)
	{
		if(imp_.get()==0)
		{
			imp_.reset(new udp_appender_imp());
			imp_->open(host_,port_);
		}
		detail::basic_stack_ostream<char,LOGGING_MAX_MESSAGE_SIZE+512>	out;

		_get_layout().format(event,out.get_out());
		imp_->append(out.str(),out.length());
	}

	shared_ptr<udp_appender> udp_appender::create( const std::string& name,layout* l,const std::string& host ,uint16_t port )
	{
		shared_ptr<udp_appender> p( new udp_appender(name,l,host,port) );
		appender::add_appender( p );
		return p;
	}


	shared_ptr<appender> udp_appender_creator::create_appender( const std::string& name, const property_tree::ptree& params )
	{
		std::string host;

		host = params.get<std::string>("host","");
		if(host.empty())
			host = params.get<std::string>("ip","127.0.0.1");
		int port = params.get<int>("port",6636);

		if(port<=0||port>0xffff)
			throw std::range_error("UDP port out of range.");

		std::auto_ptr<layout> l = layout_factory::get_instance().create(params.get_child("layout"));

		if(appender::get_appender(name))
			throw std::invalid_argument(std::string("Appender name \"")+name+"\" be existed.");

		shared_ptr<udp_appender> pAppender = udp_appender::create(name,l.release(),host,port);
		return pAppender;
	}

	std::string udp_appender_creator::get_name() const
	{
		return "udp_appender";
	}
}

