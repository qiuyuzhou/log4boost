/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef __LOGGING_UDP_RECEIVER_HPP___
#define __LOGGING_UDP_RECEIVER_HPP___


#include <log4boost/receiver/receiver.hpp>

#include <boost/scoped_ptr.hpp>

namespace log4boost
{
	class udp_receiver_impl;

	/**
	 *
	 */
	class LOG4BOOST_EXPORT udp_receiver
		: public receiver
	{
	public:
		static shared_ptr<udp_receiver> create( const std::string& name );

		void open( const std::string& host, uint16_t port, boost::asio::io_service* io_service_p = 0 );

		/**
		 *	Close the receiver.
		 *
		 *	When using external io_service, must be invoked in the thread of running the io_service.
		 *	When using internal io_service, could be invoked in any thread.
		 */
		virtual void close();

	private:
		udp_receiver( const std::string& name );


		boost::scoped_ptr<udp_receiver_impl>	impl_;
	};


}



#endif//__LOGGING_UDP_RECEIVER_HPP___

