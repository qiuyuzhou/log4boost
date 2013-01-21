/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"
#include "log4boost/receiver/udp_receiver.hpp"

#include <sstream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "log4boost/detail/singleton.hpp"

namespace log4boost
{
	class udp_receiver_impl
	{
	public:
		udp_receiver_impl( udp_receiver& _ur, boost::asio::io_service* io_service_p );
		void open( const std::string& host, uint16_t port );
		void close();
	private:
		boost::asio::io_service& get_io_service()
		{
			if ( p_external_io_service_ )
			{
				return *p_external_io_service_;
			}
			else
			{
				return *io_service_;
			}
		}

		void svc();
		void handle_read( const boost::system::error_code& error, std::size_t bytes_transferred );

		boost::scoped_ptr<boost::thread>	thread_;

		boost::scoped_ptr<boost::asio::io_service>			io_service_;
		boost::asio::io_service*		p_external_io_service_;

		boost::scoped_ptr<boost::asio::ip::udp::socket>	socket_;

		udp_receiver&	udp_receiver_;
		boost::asio::ip::udp::endpoint	sender_endpoint_;
		char	buffer_[4*1024];
	};

	udp_receiver_impl::udp_receiver_impl( udp_receiver& _ur, boost::asio::io_service* io_service_p )
		:udp_receiver_(_ur)
		,p_external_io_service_(io_service_p)
	{
		if ( !io_service_p )
		{
			io_service_.reset( new boost::asio::io_service() );
		}
		socket_.reset( new boost::asio::ip::udp::socket( get_io_service() ) );
	}

	void udp_receiver_impl::svc()
	{
		get_io_service().run();
	}

	void udp_receiver_impl::open( const std::string& host, uint16_t port )
	{
		if (host.empty())
		{
			socket_->open( boost::asio::ip::udp::v4() );
			socket_->bind( boost::asio::ip::udp::endpoint( boost::asio::ip::address_v4::any(), port ) );
		}
		else
		{
			boost::asio::ip::udp::resolver	resolver(get_io_service());
			boost::asio::ip::udp::resolver::query	query( host, boost::lexical_cast<std::string>(port) );
			boost::asio::ip::udp::resolver::iterator itr = resolver.resolve(query);
			boost::asio::ip::udp::resolver::iterator	end_itr;

			if (itr!=end_itr)
			{
				socket_->open( boost::asio::ip::udp::v4() );
				socket_->bind( *itr );
			}
			else
			{
				throw std::runtime_error( std::string("Can't resolve the host: ") + host );
			}
		}

		socket_->async_receive_from( boost::asio::buffer(buffer_),sender_endpoint_,boost::bind(&udp_receiver_impl::handle_read,this,_1,_2));

		if ( io_service_ )
		{
			thread_.reset( new boost::thread( boost::bind(&udp_receiver_impl::svc,this) ) );
		}
	}

	void udp_receiver_impl::close()
	{
		if ( thread_ )
		{
			get_io_service().post( boost::bind( &boost::asio::ip::udp::socket::close,socket_.get() ) );
			thread_->join();
			thread_.reset();
		}
		else
		{
			socket_->close();
		}
	}

	void udp_receiver_impl::handle_read( const boost::system::error_code& error, std::size_t bytes_transferred )
	{
		if (!error)
		{
			try
			{
				logging_event	le;
				load( le,buffer_,bytes_transferred );

				udp_receiver_.signal_logging_event()(le);
			}
			catch(...)
			{}
		}
		else if ( error == boost::asio::error::operation_aborted)
		{
			return;
		}

		socket_->async_receive_from( boost::asio::buffer(buffer_),sender_endpoint_,boost::bind(&udp_receiver_impl::handle_read,this,_1,_2));
	}

	//-------------------------------------------------------------------------------------------------------------------------------

	udp_receiver::udp_receiver( const std::string& name )
		:receiver( name )
	{
		
	}

	void udp_receiver::open( const std::string& host, uint16_t port, boost::asio::io_service* io_service_p )
	{
		mutex::scoped_lock	lock(mutex_);
		impl_.reset( new udp_receiver_impl( *this, io_service_p ) );
		impl_->open( host, port );
	}

	shared_ptr<udp_receiver> udp_receiver::create( const std::string& name )
	{
		shared_ptr<udp_receiver>	p( new udp_receiver(name) );

		receiver::add_receiver( p );

		return p;
	}

	void udp_receiver::close()
	{
		mutex::scoped_lock	lock(mutex_);
		if (impl_)
		{
			impl_->close();
			impl_.reset();
		}
	}
}

