/* Copyright (C) 2007 - 2008  Qiu Yuzhou
*	
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef LOGGING_THREAD_ID_HPP___
#define LOGGING_THREAD_ID_HPP___

#include <boost/config.hpp>

#include <boost/thread/tss.hpp>

#if defined(BOOST_HAS_PTHREADS)
#	include <pthread.h>
#endif


namespace log4boost
{
	namespace detail
	{
		struct LOG4BOOST_EXPORT thread_name
		{
			inline static const std::string& get();
		};

		const std::string& thread_name::get()
		{
			static boost::thread_specific_ptr<std::string>	current_thread_name_;

			if(current_thread_name_.get()==0)
			{
#if defined(WIN32) || defined(WIN64)
				current_thread_name_.reset( new std::string( boost::lexical_cast<std::string>(GetCurrentThreadId()) ) );
#elif  defined(BOOST_HAS_PTHREADS)
				current_thread_name_.reset( new std::string( boost::lexical_cast<std::string>(pthread_self()) ) );
#else
				current_thread_name_.reset( new std::string() );
#endif
			}
			return *current_thread_name_;
		}
	}

}

#endif//LOGGING_THREAD_ID_HPP___

