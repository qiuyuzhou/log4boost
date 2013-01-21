/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef __LOGGING_IPC_RECEIVER_HPP___
#define __LOGGING_IPC_RECEIVER_HPP___

#include <log4boost/detail/shared_ptr.hpp>
#include <log4boost/receiver/receiver.hpp>

#include <boost/scoped_ptr.hpp>
#include <log4boost/detail/mutex.hpp>

namespace boost{  
	namespace interprocess{

	class message_queue;

	}
	class thread;
}

namespace log4boost
{

	class LOG4BOOST_EXPORT ipc_receiver
		: public receiver
	{
	public:
		virtual ~ipc_receiver();

		static shared_ptr<ipc_receiver>	create( const std::string& receiver_name );

		void open( const std::string& messsage_queue_name, unsigned int max_msg=512,unsigned int max_msg_size=1024 );

		virtual void close();

	private:
		ipc_receiver( const std::string& name );

		void svc();

		boost::scoped_ptr<boost::interprocess::message_queue>	message_queue_;
		boost::scoped_ptr<boost::thread>						thread_;
		unsigned int											max_msg_size_;
		bool													closed_;
	};


}


#endif//__LOGGING_IPC_RECEIVER_HPP___
