/* Copyright (C) 2007 - 2008  Qiu Yuzhou
 *	
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include "stdafx.h"
#include "log4boost/appender/ipc_appender.hpp"
#include "log4boost/configurator.hpp"

#include "ptree_inc.h"
#include "log4boost/detail/stack_ostream.hpp"

#include <boost/interprocess/ipc/message_queue.hpp>

namespace log4boost
{

	ipc_appender::ipc_appender( const std::string& name,const std::string& ipc_name ,unsigned int max_msg,unsigned int max_msg_size, layout* _layout )
		:layout_appender(name,_layout)
		,ipc_name_(ipc_name)
		,max_msg_(max_msg)
		,max_msg_size_(max_msg_size)
	{
		using namespace boost::interprocess;
		try
		{
			message_queue::remove( ipc_name.c_str() );
		}
		catch(...)
		{
		}
		message_queue_.reset(new message_queue(open_or_create,ipc_name_.c_str(),max_msg_,max_msg_size_));
	}

	ipc_appender::~ipc_appender()
	{
	}

	void ipc_appender::_append( const logging_event& event )
	{
		if(!message_queue_)
			return;

		detail::stack_ostream	buf;

		_get_layout().format(event,buf.get_out());
		message_queue_->try_send(buf.str(),max_msg_size_<buf.length()?max_msg_size_:buf.length(),0);
	}

	void ipc_appender::reopen()
	{
		using namespace boost::interprocess;

		message_queue_.reset();
		message_queue_.reset(new message_queue(open_or_create,ipc_name_.c_str(),max_msg_,max_msg_size_));
	}

	void ipc_appender::close()
	{
		message_queue_.reset();
	}

	shared_ptr<ipc_appender> ipc_appender::create( const std::string& name,const std::string& ipc_name ,unsigned int max_msg/*=512*/,unsigned int max_msg_size/*=1024*/, layout* _layout /*= 0 */ )
	{
		shared_ptr<ipc_appender>	p( new ipc_appender( name, ipc_name, max_msg, max_msg_size, _layout ) );
		appender::add_appender( p );
		return p;
	}


	shared_ptr<appender> ipc_appender_creator::create_appender( const std::string& name, const property_tree::ptree& params )
	{
		if(appender::get_appender(name))
			throw std::invalid_argument(std::string("Appender name \"")+name+"\" duplicate.");

		std::string ipc_name;
		try
		{
			ipc_name = params.get<std::string>("ipc_name");
		}
		catch(const property_tree::ptree_error& e)
		{
			(e);
			throw configure_failure((boost::format("Need ipc_name in ipc_appender named %1%")%name).str());
		}
		unsigned int max_msg,max_msg_size;
		max_msg = params.get<unsigned int>("max_msg",512);
		max_msg_size = params.get<unsigned int>("max_msg_size",1024);

		shared_ptr<ipc_appender>	pAppender = ipc_appender::create(name,ipc_name,max_msg,max_msg_size);
		return pAppender;
	}

	std::string ipc_appender_creator::get_name() const
	{
		return "ipc_appender";
	}
}
