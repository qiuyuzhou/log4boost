#ifndef _LOG4BOOST_TCP_RECEIVER_HPP__
#define _LOG4BOOST_TCP_RECEIVER_HPP__

#include <log4boost/receiver/receiver.hpp>
#include <log4boost/detail/shared_ptr.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/asio.hpp>

namespace log4boost
{

	class tcp_receiver_impl;

	class LOG4BOOST_EXPORT tcp_receiver
		:public receiver
	{
	public:
		static shared_ptr<tcp_receiver> create( const std::string& name );

		void open( const std::string& host, uint16_t port, boost::asio::io_service* io_service_p = 0 );

		/**
		*	Close the receiver.
		*
		*	When using external io_service, must be invoked in the thread of running the io_service.
		*	When using internal io_service, could be invoked in any thread.
		*/
		virtual void close();
	protected:
		tcp_receiver( const std::string& name );

	private:
		boost::scoped_ptr<tcp_receiver_impl>	impl_;

	};





}





#endif