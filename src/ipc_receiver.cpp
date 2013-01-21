/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/receiver/ipc_receiver.hpp"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread/thread.hpp>

namespace log4boost
{
	
	ipc_receiver::ipc_receiver( const std::string& name )
		:receiver(name)
		,closed_(true)
	{

	}

	void ipc_receiver::close()
	{
		mutex::scoped_lock	lock(mutex_);
		if (!closed_)
		{
			closed_ = true;
			thread_->join();
			thread_.reset();
			message_queue_.reset();
		}
	}

	void ipc_receiver::open( const std::string& messsage_queue_name, unsigned int max_msg/*=512*/,unsigned int max_msg_size/*=1024 */ )
	{
		mutex::scoped_lock	lock(mutex_);
		using namespace boost::interprocess;
		closed_ = false;
		max_msg_size_ = max_msg_size;

		try
		{
			message_queue::remove( messsage_queue_name.c_str() );
		}
		catch(...)
		{
		}

		message_queue_.reset( new message_queue(open_or_create,messsage_queue_name.c_str(),max_msg,max_msg_size) );
		thread_.reset( new boost::thread( boost::bind(&ipc_receiver::svc,this) ) );
	}

	void ipc_receiver::svc()
	{
		std::string		buffer;
		buffer.resize( max_msg_size_ );

		while (true)
		{
			try
			{

				std::size_t	recvd_size;
				unsigned int priority;
				if ( message_queue_->timed_receive( &buffer[0],buffer.size(),recvd_size,priority, boost::posix_time::microsec_clock::universal_time() + boost::posix_time::millisec(200) ) )
				{
					logging_event	le;

					mutex::scoped_lock	lock(mutex_);

					if (!signal_logging_event().empty())
					{
						load( le,&buffer[0],recvd_size );
						signal_logging_event()(le);
					}
				}

				if ( closed_ )
				{
					break;
				}
			}
			catch(...)
			{}
		}
	}

	shared_ptr<ipc_receiver> ipc_receiver::create( const std::string& receiver_name )
	{
		shared_ptr<ipc_receiver> p( new ipc_receiver(receiver_name) );
		receiver::add_receiver( p );
		return p;
	}

	ipc_receiver::~ipc_receiver()
	{

	}
}

