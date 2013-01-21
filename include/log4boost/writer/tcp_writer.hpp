/* Copyright (C) 2007 - 2010  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef __LOG4BOOST_TCP_WRITER_HPP__3297690__
#define __LOG4BOOST_TCP_WRITER_HPP__3297690__

#include <log4boost/config.hpp>

#include <log4boost/detail/mutex.hpp>
#include <log4boost/writer/writer.hpp>


#include <boost/circular_buffer.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/condition.hpp>
#include <boost/asio.hpp>


namespace log4boost
{


	class LOG4BOOST_EXPORT tcp_writer
		: public writer
	{
	public:
		typedef boost::system::error_code error_code;
		tcp_writer( int reconnection_delay_seconds );

		//void open( const boost::asio::ip::tcp::endpoint& endpoint );

		void open( const std::string& host, uint16_t port );

		virtual void close();

		virtual void write(const char* str,std::size_t length);

		void set_reconnection_delay( int seconds )
		{
			reconnection_delay_ = seconds;
		}

	protected:
		bool sync_start();

		void svc();

		void start_send();
		void handle_send( const error_code& error, std::size_t bytes_transferred );

		void start_read();
		void handle_read( const error_code& error, std::size_t bytes_transferred );

		void start_timer();
		void handle_timer( const error_code& error );

		void close_i();


	private:
		std::string		host_;
		uint16_t	port_;

		boost::asio::io_service			io_service_;
		boost::asio::ip::tcp::socket	socket_;
		boost::asio::ip::tcp::resolver	resolver_;
		boost::asio::ip::tcp::endpoint	endpoint_;
		boost::asio::deadline_timer		timer_;

		boost::scoped_ptr<boost::asio::io_service::work>	work_;


		boost::scoped_ptr<boost::thread>	thread_;

		boost::circular_buffer<std::string>	buffer_;
		std::string		sending_buffer_;
		mutex			buffer_mutex_;

		boost::condition			cond_;

		int				reconnection_delay_;
		char			dummy_read_buffer_[1024*4];

		bool			closed_;
	};


}


#endif//__LOG4BOOST_TCP_WRITER_HPP__3297690__

