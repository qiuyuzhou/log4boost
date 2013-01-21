/* Copyright (C) 2007 - 2010  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#include "stdafx.h"

#include "log4boost/log4boost.hpp"
#include "log4boost/writer/tcp_writer.hpp"

#include <boost/bind.hpp>


namespace log4boost
{


	tcp_writer::tcp_writer( int reconnection_delay_seconds )
		: socket_( io_service_ )
		, resolver_( io_service_ )
		, timer_( io_service_ )
		, buffer_( 1024*1024 )
		, reconnection_delay_( reconnection_delay_seconds )
		, port_(0)
		, closed_(true)
	{
	}

	void tcp_writer::open( const std::string& host, uint16_t port )
	{
		if ( thread_ )
		{
			throw std::logic_error("Ther tcp writer has been opened.");
		}

		buffer_.clear();
		host_ = host;
		port_ = port;

		closed_ = false;

		if ( !sync_start() )
		{
			start_timer();
		}

		work_.reset( new boost::asio::io_service::work(io_service_) );
		thread_.reset( new boost::thread( boost::bind( &tcp_writer::svc, this ) ) );

	}

	bool tcp_writer::sync_start()
	{
		error_code	ec;

		boost::asio::ip::tcp::resolver::query	query( boost::asio::ip::tcp::v4(), host_, boost::lexical_cast<std::string>(port_) );
		boost::asio::ip::tcp::resolver::iterator iterator = resolver_.resolve(query);
		while( iterator!= boost::asio::ip::tcp::resolver::iterator() )
		{
			boost::asio::ip::tcp::endpoint	endpoint = *iterator;
			socket_.connect(endpoint,ec);
			if ( !ec )
			{
				break;
			}

			++iterator;
		}
		if ( iterator == boost::asio::ip::tcp::resolver::iterator() && ec )
		{
			return false;
		}
		start_read();
		return true;
	}

	void tcp_writer::close()
	{
		if ( thread_ )
		{
			closed_ = true;
			io_service_.post( boost::bind( &tcp_writer::close_i, this ) );
			if( thread_->joinable() )
				thread_->join();
		}
	}

	void tcp_writer::close_i()
	{
		error_code ec;
		socket_.close(ec);
		work_.reset();
		timer_.cancel( ec );
		resolver_.cancel();
	}

	void tcp_writer::svc()
	{
		io_service_.run();
	}

	void tcp_writer::start_send()
	{
		boost::asio::async_write( socket_, boost::asio::buffer(buffer_.front().data(), buffer_.front().size()),
			boost::bind( &tcp_writer::handle_send, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) 
			);
	}

	void tcp_writer::handle_send( const error_code& error, std::size_t bytes_transferred )
	{
		if ( !error )
		{
			mutex::scoped_lock	lock( buffer_mutex_ );
			if ( bytes_transferred > 0 )
			{
				buffer_.pop_front();
			}
			if ( !buffer_.empty() )
			{
				start_send();
			}
		}
		else if ( error != boost::asio::error::operation_aborted )
		{
			//start_timer();
		}
	}

	void tcp_writer::handle_timer( const error_code& error )
	{
		if ( !error )
		{
			if ( !sync_start() )
			{
				start_timer();
			}
		}
	}

	void tcp_writer::start_timer()
	{
		timer_.expires_from_now( boost::posix_time::seconds(reconnection_delay_) );
		timer_.async_wait( boost::bind( &tcp_writer::handle_timer, this, boost::asio::placeholders::error ) );
	}

	void tcp_writer::write( const char* str,std::size_t length )
	{
		if ( length == 0 )
		{
			return;
		}

		mutex::scoped_lock	lock( buffer_mutex_ );
		bool bEmpty = buffer_.empty();
		if ( buffer_.full() )
		{
			return;
		}
		std::string data(str,length);
		buffer_.push_back( std::string() );
		buffer_.back().swap(data);
		if ( bEmpty )
		{
			io_service_.post( boost::bind( &tcp_writer::handle_send, this, error_code(), 0 ) );
		}
	}

	void tcp_writer::start_read()
	{
		socket_.async_read_some(
			boost::asio::buffer( dummy_read_buffer_ )
			, boost::bind( &tcp_writer::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) 
			);
	}

	void tcp_writer::handle_read( const error_code& error, std::size_t bytes_transferred )
	{
		if ( !error )
		{
			socket_.async_read_some(
				boost::asio::buffer( dummy_read_buffer_ )
				, boost::bind( &tcp_writer::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) 
				);
		}
		else if ( error != boost::asio::error::operation_aborted )
		{
			if ( !closed_ )
			{
				start_timer();
			}
		}
	}
}

